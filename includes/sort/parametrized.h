#pragma once

#include <concepts>
#include <utility>

#include <cstddef>

namespace Sort
{
    class IParameterized
    {
    public:
        virtual ~IParameterized() noexcept                                       = default;
        virtual std::pair<std::int64_t, std::int64_t> getParameterBounds() const = 0;
        virtual void                                  setParameter(int value)    = 0;
    };

    template <typename T>
    concept arithmetic = std::integral<T> || std::floating_point<T>;

    template <typename T>
    requires arithmetic<T>
    struct Parameter
    {
        using Type = T;
        T value {0};
        T minVal {0};
        T maxVal {0};
    };

    template <typename T>
    requires arithmetic<T>
    class Parameterized : public IParameterized
    {
    private:
        Parameter<T> m_param;

    public:
        using ElemType                    = T;

        Parameterized() noexcept          = default;
        virtual ~Parameterized() noexcept = default;

        std::pair<std::int64_t, std::int64_t> getParameterBounds() const override
        {
            return {static_cast<std::int64_t>(m_param.minVal), static_cast<std::int64_t>(m_param.maxVal)};
        }

        void setParameter(int value) override
        {
            m_param.value = std::clamp<T>(static_cast<T>(value), m_param.minVal, m_param.maxVal);
        }

        inline void setParameterBounds(T minVal, T maxVal)
        {
            m_param.minVal = minVal;
            m_param.maxVal = maxVal;
        }

        inline T const getParameter() const { return m_param.value; }
    };
}  // namespace Sort
