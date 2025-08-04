#pragma once
#include <vector>
#include <cstddef>

namespace Sort
{
    template<typename T>
    class SortArray 
    {
    private:
        std::vector<T> data;
        mutable std::size_t accessCount = 0;
        mutable std::size_t swapCount = 0;
        mutable std::size_t compCount  = 0;
    public:
        SortArray() = default;
        SortArray(std::initializer_list<T> init)
        {
            for (const auto& v : init)
            {
                add(v);
            }
        }

        void add(const T& v)
        { 
            data.emplace_back(v);
        }

        std::size_t size() const noexcept
        {
            return data.size();
        }

        Elem& operator[](size_t i)
        { 
            ++accessCount;
            return data[i];
        }

        const Elem& operator[](size_t i) const
        {
            ++accessCount;
            return data[i];
        }

        void swap(size_t a, size_t b)
        {
            ++swapCount;
            std::swap(data[a], data[b]);
        }

        std::size_t getAccesses() const noexcept
        { 
            return accessCount;
        }

        std::size_t getSwaps() const noexcept
        { 
            return swapCount;
        }

        std::size_t getComparisons() const noexcept
        { 
            return compCount;
        }

        void resetCounters() noexcept
        {
            accessCount = swapCount = compCount = 0;
        }

        auto begin()
        { 
            return data.begin();
        }
        
        auto end()
        { 
            return data.end();
        }

        auto begin() const
        { 
            return data.begin();
        }
        
        auto end() const
        { 
            return data.end();
        }
    };
}