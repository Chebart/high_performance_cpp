#pragma once
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <optional>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class OwnHashMap {
private:
    struct Entry {
        Key key;
        Value value;
    };

    std::vector<std::list<Entry>> table;
    size_t num_elements;
    float max_load_factor;

    void rehash();

public:
    OwnHashMap(size_t initial_size = 16, float load_factor = 0.75f);

    void insert(const std::pair<Key, Value>& kv);
    std::optional<Value> find(const Key& key) const;
    bool erase(const Key& key);
    size_t size() const;
};