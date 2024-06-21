#include <benchmark/benchmark.h>
#include <random>
#include "../src/table.h"
#include "bounds.h"



static void unordered_map_insert(std::unordered_map<uint64_t, uint64_t> & t, size_t sz) {
    RNG_INIT
    BOUND(i, sz) {
        uint64_t k = MAKE_KEY(i);
        uint64_t v = MAKE_VALUE(i);
        t[k] = v;
    }
}

static void ffht_insert(Table<uint64_t, uint64_t> & t, size_t sz) {
    RNG_INIT
    BOUND(i, sz) {
        uint64_t k = MAKE_KEY(i);
        uint64_t v = MAKE_VALUE(i);
        t.insert(k, v);
    }
}

static void insert_bench(benchmark::State& state) {
    Table<uint64_t, uint64_t> t(16);
    for (auto _ : state) {
        ffht_insert(t, state.range(0));
    }
}


static void unordered_map_cmp(benchmark::State& state) {
    std::unordered_map<uint64_t, uint64_t> t;
    for (auto _ : state) {
        unordered_map_insert(t, state.range(0));
    }
}


static void unordered_map_rng_insert(std::unordered_map<uint64_t, uint64_t> & t, size_t sz) {
    RNG_INIT
    BOUND(i, sz) {
        uint64_t k = MAKE_RANDOM_KEY(i);
        uint64_t v = MAKE_RANDOM_VALUE(i);
        t[k] = v;
    }
}

static void ffht_rng_insert(Table<uint64_t, uint64_t> & t, size_t sz) {
    RNG_INIT
    BOUND(i, sz) {
        uint64_t k = MAKE_RANDOM_KEY(i);
        uint64_t v = MAKE_RANDOM_VALUE(i);
        t.insert(k, v);
    }
}

static void insert_rng_bench(benchmark::State& state) {
    Table<uint64_t, uint64_t> t(16);
    for (auto _ : state) {
        ffht_rng_insert(t, state.range(0));
    }
}


static void unordered_map_rng_cmp(benchmark::State& state) {
    std::unordered_map<uint64_t, uint64_t> t;
    for (auto _ : state) {
        unordered_map_rng_insert(t, state.range(0));
    }
}

//random ones
BENCHMARK(insert_rng_bench)->Arg(10);
BENCHMARK(unordered_map_rng_cmp)->Arg(10);

BENCHMARK(insert_rng_bench)->Arg(100);
BENCHMARK(unordered_map_rng_cmp)->Arg(100);

BENCHMARK(insert_rng_bench)->Arg(1000);
BENCHMARK(unordered_map_rng_cmp)->Arg(1000);

BENCHMARK(insert_rng_bench)->Arg(10000);
BENCHMARK(unordered_map_rng_cmp)->Arg(10000);

BENCHMARK(insert_rng_bench)->Arg(100000);
BENCHMARK(unordered_map_rng_cmp)->Arg(100000);

BENCHMARK(insert_rng_bench)->Arg(1000000);
BENCHMARK(unordered_map_rng_cmp)->Arg(1000000);

BENCHMARK(insert_rng_bench)->Arg(10000000);
BENCHMARK(unordered_map_rng_cmp)->Arg(10000000);

BENCHMARK(insert_rng_bench)->Arg(100000000);
BENCHMARK(unordered_map_rng_cmp)->Arg(100000000);

// Non random ones
BENCHMARK(insert_bench)->Arg(10);
BENCHMARK(unordered_map_cmp)->Arg(10);

BENCHMARK(insert_bench)->Arg(100);
BENCHMARK(unordered_map_cmp)->Arg(100);

BENCHMARK(insert_bench)->Arg(1000);
BENCHMARK(unordered_map_cmp)->Arg(1000);

BENCHMARK(insert_bench)->Arg(10000);
BENCHMARK(unordered_map_cmp)->Arg(10000);

BENCHMARK(insert_bench)->Arg(100000);
BENCHMARK(unordered_map_cmp)->Arg(100000);

BENCHMARK(insert_bench)->Arg(1000000);
BENCHMARK(unordered_map_cmp)->Arg(1000000);

BENCHMARK(insert_bench)->Arg(10000000);
BENCHMARK(unordered_map_cmp)->Arg(10000000);

BENCHMARK(insert_bench)->Arg(100000000);
BENCHMARK(unordered_map_cmp)->Arg(100000000);


BENCHMARK_MAIN();
