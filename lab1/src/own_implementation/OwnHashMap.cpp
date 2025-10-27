#include <string>
#include "OwnHashMap.hpp"
#include "hash.hpp"

template <typename Key, typename Value, typename Hash>
OwnHashMap<Key, Value, Hash>::OwnHashMap(size_t initial_size, float load_factor)
    : table(initial_size), num_elements(0), max_load_factor(load_factor) {}

template <typename Key, typename Value, typename Hash>
void OwnHashMap<Key, Value, Hash>::rehash() {
    size_t new_size = table.size() * 2;
    std::vector<std::list<Entry>> new_table(new_size);

    for (auto& bucket : table) {
        for (auto& entry : bucket) {
            size_t idx = Hash{}(entry.key) % new_size;
            new_table[idx].push_back(entry);
        }
    }

    table.swap(new_table);
}

template <typename Key, typename Value, typename Hash>
void OwnHashMap<Key, Value, Hash>::insert(const std::pair<Key, Value>& kv) {
    if (num_elements + 1 > table.size() * max_load_factor) {
        rehash();
    }

    size_t idx = Hash{}(kv.first) % table.size();
    for (auto& entry : table[idx]) {
        if (entry.key == kv.first) {
            entry.value = kv.second;
            return;
        }
    }
    table[idx].push_back({kv.first, kv.second});
    num_elements++;
}

template <typename Key, typename Value, typename Hash>
std::optional<Value> OwnHashMap<Key, Value, Hash>::find(const Key& key) const {
    size_t idx = Hash{}(key) % table.size();
    for (const auto& entry : table[idx]) {
        if (entry.key == key) {
            return entry.value;
        }
    }
    return std::nullopt;
}

template <typename Key, typename Value, typename Hash>
bool OwnHashMap<Key, Value, Hash>::erase(const Key& key) {
    size_t idx = Hash{}(key) % table.size();
    auto& bucket = table[idx];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->key == key) {
            bucket.erase(it);
            num_elements--;
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value, typename Hash>
size_t OwnHashMap<Key, Value, Hash>::size() const {
    return num_elements;
}

template class OwnHashMap<int, std::string, BadHash>;
template class OwnHashMap<int, std::string, MediumHash>;
template class OwnHashMap<int, std::string, GoodHash>;