#pragma once

#include <random>

#define RNG_INIT std::random_device rd;\
    std::mt19937 gen(rd());\
    std::uniform_int_distribution<> distr(10, 100000000);\

#define BOUND(var_name, size) for (int var_name = 0; var_name < size; var_name ++)
#define MAKE_RANDOM_KEY(index_name) distr(gen)
#define MAKE_RANDOM_VALUE(index_name) distr(gen)
#define MAKE_KEY(index_name) index_name
#define MAKE_VALUE(index_name) index_name

// 3617511
