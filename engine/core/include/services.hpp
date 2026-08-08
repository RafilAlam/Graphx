#pragma once

#include <engine/graphics/include/rendering/debugrenderer.hpp>
#include <unordered_map>
#include <cassert>
#include <typeindex>

class Services {
public:
    static Services& Get() {
        static Services instance;
        return instance;
    }

    template<typename T>
    static void Set(T& service) {
        Get().m_services[typeid(T)] = &service;
    }

    template<typename T>
    static T& Get() {
        auto it = Get().m_services.find(typeid(T));
        assert(it != Get().m_services.end() && "Requested service has not been registered.");
        return *static_cast<T*>(it->second);
    }

    Services(const Services&) = delete;
private:
    Services() = default;
    std::unordered_map<std::type_index, void*> m_services;
};