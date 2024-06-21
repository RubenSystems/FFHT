# FFHT

FFHT is going to be a hybrid linear/quadratic probing table. 

Currently it is only a linear probing table. 


The aim is to both reduce lookup latency and increase the max load factor.


Current benchmarking data: 

```
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
insert_rng_bench/10                   1772 ns         1768 ns       440083
unordered_map_rng_cmp/10              3143 ns         3140 ns       287979
insert_rng_bench/100                  8560 ns         8543 ns        98597
unordered_map_rng_cmp/100            25865 ns        25810 ns        45757
insert_rng_bench/1000                73331 ns        73178 ns        11852
unordered_map_rng_cmp/1000          240355 ns       239687 ns         5195
insert_rng_bench/10000              716263 ns       712180 ns         1198
unordered_map_rng_cmp/10000        2386699 ns      2375519 ns          501
insert_rng_bench/100000            7224925 ns      7186958 ns          120
unordered_map_rng_cmp/100000      24183599 ns     24055887 ns           53
insert_rng_bench/1000000          71357610 ns     70912545 ns           11
unordered_map_rng_cmp/1000000    239822300 ns    237953800 ns            5
insert_rng_bench/10000000        728387042 ns    726437000 ns            1
unordered_map_rng_cmp/10000000  2782503917 ns   2773936000 ns            1
```
