#include <cstring>
#include <stddef.h>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utility>

#include <arm_neon.h>

#include <iostream>

#define DEBUG false

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

using namespace std;

#define SLOT_EMPTY          0b00000000
#define SLOT_FULL_MASK      0b10000000
#define MAX_LOAD_FACTOR 0.7

// #define HIGH_KEY(val)       (val & ~0b1111111)
// #define H1(val) (val)
#define SLOT_FULL(val) (SLOT_FULL_MASK | (val & 0b1111111))

#define GROUP_SIZE 16

#define MAX_JUMP_COUNT 1000000

template <typename Ok>
struct Option {
    Option(): tag(false) {}
    Option(Ok v): tag(true), ok_(v) {}

    bool is_some() {return tag;}

    inline Ok some() {
        #if DEBUG
            if (!tag) {
                printf("PANIC! unwrap option\n");
                exit(0);
            }
        #endif
        return ok_;
    }

    private:
        bool tag;
        Ok ok_;

};

template <typename OneT, typename TwoT>
struct Two {
    OneT one;
    TwoT two;
};

template <typename KeyT, typename ValT>
struct Table {
    /*
        To speed up probing, control data stored seperatly
        to the table data. Currently control data are represented
        by an 8 bit unsigned integer. The highest bit signifies that
        a slot is full:

                    0b1XXXXXXX --> full
                    0b0XXXXXXX --> empty

        The lowest seven bits of a full slot stores the lowst seven
        bits of a hash. For example the following n bit key would
        be reperesented as follows:

                    hash: ...001010101 |     0101101
                    upper: ^^^^^^^^^^  lower ^^^^^^^

                ctrl:
                    0b10101101


        The upper hash is then used to determine what group to
        start the probe.
    */
    typedef uint8_t ctrl_t;
    typedef Two<KeyT, ValT> Slot;


    Table(size_t slot_c) :
        remaining(slot_c * MAX_LOAD_FACTOR),
        slot_c(slot_c)
    {
        // Allocate buffer for control bytes and mark all slots as empty.
        control = (ctrl_t *)malloc(sizeof(ctrl_t) * slot_c);
        memset(control, SLOT_EMPTY, sizeof(ctrl_t) * slot_c);

        slots = (Slot *)malloc(sizeof(Slot) * slot_c);
    }

    ~Table() {
        free(control);
        free(slots);
    }

    // No copying yet
    Table(const Table &)               = delete;
    Table * operator=(const Table &)   = delete;

    // Moving is possible and used to ease resizing logic
    Table(Table && rhs) {
        *this = rhs;
    }

    Table * operator=(Table && rhs) {
        remaining = rhs.remaining;
        slot_c = rhs.slot_c;
        swap(control, rhs.control);
        swap(slots, rhs.slots);
        return this;
    }


    // functions
    public:
        void insert(KeyT key, ValT value){

            if (remaining <= 0){
                resize();

            }
            // Aligned down upper hash
            size_t h1_loc = key;
            size_t idx = 0;
            do {
                size_t probe_idx = (h1_loc + idx) % slot_c;
                uint8_t & ctrl_b = control[probe_idx];
                if (ctrl_b == SLOT_FULL(key) && slots[probe_idx].one == key) {
                    slots[probe_idx].two = value;
                    goto done;
                } else if (ctrl_b == SLOT_EMPTY) {
                    ctrl_b = SLOT_FULL(key);
                    remaining--;

                    Slot & s = slots[probe_idx];
                    s.one = key;
                    s.two = value;
                    goto done;
                }
                idx++;
            } while (1);
            done:
                return;
        }

        Option<ValT> get(KeyT key) {
            size_t h1_loc = key;
            size_t idx = 0;
            do {
                size_t probe_idx = (h1_loc + idx) % slot_c;
                uint8_t & ctrl_b = control[probe_idx];
                if (ctrl_b == SLOT_FULL(key) && slots[probe_idx].one == key) {
                    return Option<ValT>(slots[probe_idx].two);
                } else if (ctrl_b == SLOT_EMPTY) {
                    return Option<ValT>();
                }
                idx++;
            } while (1);
        }

    private:
        void resize(){
            Table<KeyT, ValT> tmp (slot_c * 2);
            for (size_t i = 0; i < slot_c; i ++) {
                if (control[i] & SLOT_FULL_MASK) {
                    Slot & s = slots[i];
                    tmp.insert(s.one, s.two);
                }
            }
            *this = std::move(tmp);
        }

        void debug_print() {
            cout << "=======================\n";
            cout << "Capacity: " << slot_c;
            cout << " Remaining: " << remaining << endl;
            for (int i = 0; i < slot_c; i ++) {
                Slot & g = slots[i];
                cout << (int)control[i] << " " << g.one << ": " << g.two << endl;
            }
            cout << "=======================\n";
        }



    private:
        int64_t     remaining;
        size_t      slot_c;
        ctrl_t *    control;
        Slot *      slots;
};

// template <typename KeyT, typename ValT>
// struct Pair {
//     KeyT k;
//     ValT v;
// };

// template <typename KeyT, typename ValT>
// struct Table {

//     typedef Pair<KeyT, ValT> SlotT ;

//     Table (size_t sz) :
//         buffer(
//             (uint8_t *)malloc((sizeof(SlotT) + sizeof(uint8_t)) * sz)
//         ),
//         remaining_cap_(sz * MAX_LOAD_FACTOR),
//         capacity_(sz)
//     {
//         memset(buffer, 0, sizeof(uint8_t) * sz);
//         slot_offset = (SlotT *)((sizeof(uint8_t) * sz) + buffer);
//     }

//     ~Table() {
//         free(buffer);
//     }

//     // Delete copy constructer/assignment overload
//     Table(const Table &) = delete;
//     Table * operator = (const Table &) = delete;

//     // Implement move construcor/assignment overload
//     Table(Table && t) {
//         *this = t;
//     }

//     Table * operator=(Table && t) {
//         std::swap(buffer, t.buffer);
//         remaining_cap_ = t.remaining_cap_;
//         capacity_ = t.capacity_;
//         slot_offset = t.slot_offset;
//         return this;
//     }


//     void insert(KeyT key, ValT val) {
//         debug_print();
//         if (unlikely(remaining_cap_ <= 0)) {
//             resize();
//         };

//         // size_t start = key % capacity_;
//         // size_t index = 1;
//         // size_t probe = start;
//         // // uint32_t jmp = 0;
//         // uint8_t full_mask = SLOT_FULL(key);

//         // do {
//         //     uint8_t & ctrl_b = buffer[probe];
//         //     if (ctrl_b == full_mask) {
//         //         SlotT & s = slot_offset[probe];
//         //         if (s.k == key) {
//         //             s.v = val;
//         //             goto done;
//         //         };
//         //     } else if (ctrl_b == SLOT_EMPTY) {
//         //         // set control byte to be full
//         //         ctrl_b = full_mask;
//         //         remaining_cap_--;

//         //         // get slot and insert
//         //         SlotT & s = slot_offset[probe];
//         //         s.k = key;
//         //         s.v = val;
//         //         goto done;
//         //     }

//         //     index++;
//         //     probe = probe_(start, index) % capacity_;
//         //     // cout << probe<<endl;
//         //     // jmp++;
//         // } while (1);
//         // done:
//         // // cout << jmp << endl;
//         // return;
//     }

//     // Option<ValT> get(KeyT key) {
//     //     size_t i = key % capacity_;
//     //     do {
//     //         uint8_t & ctrl_b = buffer[i];
//     //         if (ctrl_b == SLOT_FULL(key)) {
//     //             SlotT & s = slot_offset[i];
//     //             if (s.k != key) continue;
//     //             return Option<ValT>(s.v);
//     //         } else if (ctrl_b == SLOT_EMPTY) {
//     //             return Option<ValT>();
//     //         }
//     //         i = (i + 1) % capacity_;
//     //     } while (1);
//     // }


//     private:

//     void resize() {

//         Table<KeyT, ValT> nt(capacity_ * 2);
//         for (int i = 0; i < capacity_; i++) {
//             if ((buffer[i] & SLOT_FULL_MASK)){
//                 SlotT & oldslot = slot_offset[i];
//                 nt.insert(oldslot.k, oldslot.v);
//             }
//         }
//         *this = std::move(nt);
//     }

//     // Pair<SlotT *, size_t> find(KeyT key) {
//     //     size_t start = key % capacity_;
//     //     size_t i = start;
//     //     do {
//     //         uint8_t ctrl_b = buffer[i];

//     //         if (ctrl_b == SLOT_FULL(key)) {
//     //             SlotT * s = &slot_offset[i];
//     //             if (s->k == key) return {s, i};
//     //         } else if (ctrl_b == SLOT_EMPTY) {
//     //             return {&slot_offset[i], i};
//     //         }

//     //         i = (i + 1) % capacity_;
//     //     } while(1);
//     // }


//     void debug_print() {
//         cout << "=======================\n";
//         cout << "Capacity: " << capacity_;
//         cout << " Remaining: " << remaining_cap_ << endl;
//         for (int i = 0; i < capacity_; i ++) {
//             SlotT & g = slot_offset[i];
//             cout << (int)buffer[i] << " " << g.k << ": " << g.v << endl;
//         }
//         cout << "=======================\n";
//     }



//     private:
//         uint8_t * buffer;
//         SlotT * slot_offset;
//         size_t remaining_cap_;
//         size_t capacity_;
// };
