#pragma once

#include <vector>

/*
An efficient, cache-friendly data structure.
Sparse - stores index of dense data at index = entity handle
Dense - contains packed data for cache-locality
*/ 
template<typename T>
class sparseset {
public:
    std::vector<uint32_t> sparse;
    std::vector<T> dense;

    bool contains(uint32_t id) {
        return id < sparse.size() && sparse[id] != -1;
    }

    T& operator[](uint32_t id) {
        assert(contains(id) && "Entity does not exist in this sparse set!");
        return dense[sparse[id]];
    }

    const T& operator[](uint32_t id) const {
        assert(contains(id) && "Entity does not exist in this sparse set!");
        return dense[sparse[id]];
    }

    template<typename... Args>
    T& emplace_back(uint32_t id, Args&&... args) {
        if (id >= sparse.size()) {
            sparse.resize(id + 1, -1);
        }

        sparse[id] = static_cast<uint32_t>(dense.size());
        dense.emplace_back(std::forward<Args>(args)...);
        return dense.back();
    }
};