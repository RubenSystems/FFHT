
#include <iostream>
#include "src/table.h"
#include "benchmarks/bounds.h"
#include <stdexcept>
#include <unordered_map>

using namespace std;

static void ffht_insert(Table<uint64_t, uint64_t> & t, size_t sz) {
    RNG_INIT
    size_t idx = 0;
    BOUND(i, sz) {
        uint64_t k = MAKE_KEY(i);
        uint64_t v = MAKE_VALUE(i);
        t.insert(k, v);
        // if (idx++ % 10000 == 0)
        //     cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << idx << flush;
    }
    cout << endl;
}


int main() {

    Table<uint64_t, uint64_t> t(16);
    const size_t sz = 10000000;

    for (int i =0 ; i < sz; i++) {
        t.insert(i, i);
    }

    for (int i =0 ; i < sz; i++) {
        if (t.get(i).some() != i) throw std::runtime_error("FAILED");
    }



    return 0;
}
