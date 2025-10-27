#pragma once
#include <functional>
#include <cstddef>

struct BadHash {
    std::size_t operator()(int) const {
        return 42; 
    }
};

// ((key * A) % p) % m
struct MediumHash {
    static constexpr std::size_t p = 1'048'583;
    static constexpr std::size_t A = 786433;

    std::size_t operator()(int key) const {
        return (static_cast<std::size_t>(key) * A) % p;
    }
};

struct GoodHash {
    std::size_t operator()(int key) const {
        return std::hash<int>{}(key);
    }
};