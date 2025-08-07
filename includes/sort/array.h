#pragma once

#include <algorithm>
#include <atomic>
#include <vector>

#include <cstddef>

#include "core/logging/logging.h"

namespace Sort
{
    template <typename T>
    class SortArray
    {
    private:
        std::vector<T>                     m_data;
        mutable std::atomic<std::uint64_t> m_accessCount {0};
        mutable std::atomic<std::uint64_t> m_swapCount {0};
        mutable std::atomic<std::uint64_t> m_compCount {0};
        mutable std::mutex                 m_mutex;

    public:
        using value_type = T;

        SortArray()      = default;

        SortArray(std::initializer_list<T> t_initList)
        {
            for (auto const& value : t_initList)
            {
                add(value);
            }
        }

        SortArray(SortArray<T> const& t_other)
        {
            if (this != &t_other)
            {
                std::scoped_lock lock(m_mutex, t_other.m_mutex);
                m_data        = t_other.m_data;
                m_accessCount = t_other.m_accessCount.load();
                m_swapCount   = t_other.m_swapCount.load();
                m_compCount   = t_other.m_compCount.load();
            }
        }

        void add(T const& t_value)
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            m_data.emplace_back(t_value);
        }

        SortArray<T>& operator=(SortArray<T> const& t_other)
        {
            if (this != &t_other)
            {
                std::scoped_lock lock(m_mutex, t_other.m_mutex);
                m_data        = t_other.m_data;
                m_accessCount = t_other.m_accessCount.load();
                m_swapCount   = t_other.m_swapCount.load();
                m_compCount   = t_other.m_compCount.load();
            }
            return *this;
        }

        T& operator[](std::size_t t_index)
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            m_accessCount++;
            return m_data[t_index];
        }

        T const& operator[](std::size_t t_index) const
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            m_accessCount++;
            return m_data[t_index];
        }

        void swap(std::size_t t_indexA, std::size_t t_indexB)
        {
            if (t_indexA >= size() || t_indexB >= size())
            {
                LOGERR("Swap indices out of bounds: " << t_indexA << ", " << t_indexB);
                return;
            }

            m_swapCount++;
            std::swap(m_data[t_indexA], m_data[t_indexB]);
        }

        std::atomic<std::size_t> const& getAccesses() const noexcept
        {
            return m_accessCount;
        }

        std::atomic<std::size_t> const& getSwaps() const noexcept
        {
            return m_swapCount;
        }

        std::atomic<std::size_t> const& getComparisons() const noexcept
        {
            return m_compCount;
        }

        void incComparisons() noexcept
        {
            m_compCount++;
        }

        void addComparisons(std::uint64_t t_compCount) noexcept
        {
            m_compCount += t_compCount;
        }

        void resetCounters() noexcept
        {
            m_accessCount = m_swapCount = m_compCount = 0;
        }

        auto begin()
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            return m_data.begin();
        }

        auto end()
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            return m_data.end();
        }

        auto begin() const
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            return m_data.begin();
        }

        auto end() const
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            return m_data.end();
        }

        void resize(std::size_t t_size)
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            m_data.resize(t_size);
        }

        bool empty() const noexcept
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            return m_data.empty();
        }

        std::size_t size() const noexcept
        {
            std::scoped_lock<std::mutex> lock {m_mutex};
            return m_data.size();
        }
    };
}  // namespace Sort
