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
static void BM_Insert(benchmark::State& state) {
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

    int iter_cnt = 1;
    MapType<int, std::string, Hash> m;
    std::vector<double> durations_us;
    durations_us.reserve(N);
    for (auto _ : state) {
        // Fill hashmap
        for (int i = 0; i < N; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            m.insert({keys[indices[i]], values[indices[i]]});
            auto end = std::chrono::high_resolution_clock::now();
            if (iter_cnt > 1 && iter_cnt <= 2) {
                durations_us.push_back(std::chrono::duration<double, std::micro>(end - start).count());
            }
        }
        iter_cnt++;
    }
    write_durations_csv(
        "./results/ops_distr.csv", 
        "insert", 
        "OwnHashMap",
        typeid(Hash).name(), 
        durations_us
    );
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * N);
}

#define REGISTER_Insert_Map(MAP, HASH, NAME) \
    BENCHMARK_TEMPLATE(BM_Insert, MAP, HASH)->RangeMultiplier(2)->Range(1<<10, 1<<14)->Name("Insert_" NAME);

REGISTER_Insert_Map(OwnHashMap, BadHash, "BadHash");
REGISTER_Insert_Map(OwnHashMap, MediumHash, "MediumHash");
REGISTER_Insert_Map(OwnHashMap, GoodHash, "GoodHash");