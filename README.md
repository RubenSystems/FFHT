# FFHT

FFHT is going to be a hybrid linear/quadratic probing table. 

Currently it is only a linear probing table. 


The aim is to both reduce lookup latency and increase the max load factor.


Current benchmarking data: 

```
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
insert_bench/10                      58.5 ns         57.4 ns     12127302
unordered_map_cmp/10                 67.2 ns         66.9 ns     10458219
insert_bench/100                      230 ns          229 ns      3030434
unordered_map_cmp/100                 323 ns          323 ns      2177639
insert_bench/1000                    1927 ns         1922 ns       359206
unordered_map_cmp/1000               2844 ns         2839 ns       246566
insert_bench/10000                  18844 ns        18803 ns        37360
unordered_map_cmp/10000             30665 ns        30610 ns        24063
insert_bench/100000                188805 ns       188183 ns         3735
unordered_map_cmp/100000           331842 ns       331162 ns         2228
insert_bench/1000000              1906765 ns      1903180 ns          361
unordered_map_cmp/1000000         3453266 ns      3445895 ns          200
insert_bench/10000000            21620510 ns     21578077 ns           26
unordered_map_cmp/10000000       50348492 ns     50261300 ns           10
insert_bench/100000000          803186500 ns    794590000 ns            1
unordered_map_cmp/100000000    1970247625 ns   1966047000 ns            1
insert_rng_bench/10                  1703 ns         1700 ns       447493
unordered_map_rng_cmp/10             2990 ns         2985 ns       309120
insert_rng_bench/100                 8381 ns         8367 ns       101429
unordered_map_rng_cmp/100           25343 ns        25296 ns        49301
insert_rng_bench/1000               73746 ns        73600 ns        11737
unordered_map_rng_cmp/1000         240701 ns       240198 ns         5530
insert_rng_bench/10000             733420 ns       731884 ns         1205
unordered_map_rng_cmp/10000       2416361 ns      2411762 ns          547
insert_rng_bench/100000           7299912 ns      7282783 ns          120
unordered_map_rng_cmp/100000     24384015 ns     24341560 ns           50
insert_rng_bench/1000000         72550465 ns     72413250 ns           12
unordered_map_rng_cmp/1000000   241281667 ns    240186200 ns            5
insert_rng_bench/10000000       722804791 ns    721257000 ns            1
unordered_map_rng_cmp/10000000 2777559250 ns   2768985000 ns            1
```
