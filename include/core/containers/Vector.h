#pragma once

#include <memory_resource> // Current implementation uses PMR

#include <vector>

namespace Containers
{
    // Define an alias template so we can swap allocators
    template <typename T>
    // using vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
    using vector = std::vector<T>;
}