#include <benchmark/benchmark.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>

#include "hash.hpp"
#include "OwnHashMap.hpp"
#include "write_csv.hpp"


template <template <typename, typename, typename> class MapType, typename Hash>
static void BM_Find(benchmark::State& state) {
    const int N = state.range(0);

    // Set keys and values
    std::vector<int> keys(N);
    std::vector<std::string> values(N);
    for (int i = 0; i < N; ++i) {
        keys[i] = i;
        values[i] = "value_" + std::to_string(i);
    }

    // Permute keys and values
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<size_t> indices(N);
    for (size_t i = 0; i < N; ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), g);

    // Fill hashmap
    MapType<int, std::string, Hash> m;
    for (int i = 0; i < N; ++i) {
        m.insert({keys[indices[i]], values[indices[i]]});
    }

    int iter_cnt = 1;
    std::vector<double> durations_us;
    durations_us.reserve(N);
    for (auto _ : state) {
        // Measure time for erasing keys
        for (int k : keys) {
            auto start = std::chrono::high_resolution_clock::now();
            auto it = m.find(k);
            benchmark::DoNotOptimize(it);
            auto end = std::chrono::high_resolution_clock::now();
            if (iter_cnt > 1 && iter_cnt <= 2) {
                durations_us.push_back(std::chrono::duration<double, std::micro>(end - start).count());
            }
        }
        iter_cnt++;
    }
    write_durations_csv(
        "./results/ops_distr.csv", 
        "find", 
        "OwnHashMap",
        typeid(Hash).name(), 
        durations_us
    );
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * N);
}

#define REGISTER_Find_Map(MAP, HASH, NAME) \
    BENCHMARK_TEMPLATE(BM_Find, MAP, HASH)->RangeMultiplier(2)->Range(1<<10, 1<<14)->Name("Find_" NAME);

REGISTER_Find_Map(OwnHashMap, BadHash, "BadHash");
REGISTER_Find_Map(OwnHashMap, MediumHash, "MediumHash");
REGISTER_Find_Map(OwnHashMap, GoodHash, "GoodHash");