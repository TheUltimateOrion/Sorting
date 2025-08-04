#pragma once

#include <algorithm>
#include <atomic>
#include <vector>

#include <cstddef>

#include "core/logging/logging.h"

namespace Sort
{
    template<typename T>
    class SortArray 
    {
    private:
        std::vector<T> data;
        mutable std::atomic<std::size_t> accessCount {0};
        mutable std::atomic<std::size_t> swapCount {0};
        mutable std::atomic<std::size_t> compCount {0};
        mutable std::mutex mutex;
    public:
        using value_type = T;

        SortArray() = default;
        SortArray(std::initializer_list<T> init)
        {
            for (const auto& v : init)
            {
                add(v);
            }
        }

        SortArray(const SortArray<T>& other)
        {
            if (this != &other) 
            {
                std::scoped_lock lock(mutex, other.mutex);
                data = other.data;
                accessCount= other.accessCount.load();
                swapCount = other.swapCount.load();
                compCount = other.compCount.load();
            }
        }

        void add(const T& v)
        {
            std::scoped_lock<std::mutex> lock{mutex};
            data.emplace_back(v);
        }

        std::vector<T> toVec()
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data;
        }

        SortArray<T>& operator=(const SortArray<T>& other)
        {
            if (this != &other) 
            {
                std::scoped_lock lock(mutex, other.mutex);
                data = other.data;
                accessCount= other.accessCount.load();
                swapCount = other.swapCount.load();
                compCount = other.compCount.load();
            }
            return *this;
        }

        T& operator[](size_t i)
        { 
            std::scoped_lock<std::mutex> lock{mutex};
            ++accessCount;
            return data[i];
        }

        const T& operator[](size_t i) const
        {
            std::scoped_lock<std::mutex> lock{mutex};
            ++accessCount;
            return data[i];
        }

        void swap(size_t a, size_t b)
        {
            if (a >= size() || b >= size()) 
            {
                LOGERR("Swap indices out of bounds: " << a << ", " << b);
                return;
            }

            ++swapCount;
            std::swap(data[a], data[b]);
        }

        std::atomic<std::size_t>& getAccesses() const noexcept
        {
            return accessCount;
        }

        std::atomic<std::size_t>& getSwaps() const noexcept
        { 
            return swapCount;
        }

        std::atomic<std::size_t>& getComparisons() const noexcept
        { 
            return compCount;
        }

        void resetCounters() noexcept
        {
            accessCount = swapCount = compCount = 0;
        }

        auto begin()
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data.begin();
        }
        
        auto end()
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data.end();
        }

        auto begin() const
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data.begin();
        }
        
        auto end() const
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data.end();
        }

        void resize(size_t t_size)
        {
            std::scoped_lock<std::mutex> lock{mutex};
            data.resize(t_size);
        }

        bool empty() const noexcept
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data.empty();
        }

        std::size_t size() const noexcept
        {
            std::scoped_lock<std::mutex> lock{mutex};
            return data.size();
        }
    };
}