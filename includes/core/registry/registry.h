#pragma once

#include <concepts>
#include <map>
#include <string>
#include <utility>
#include <vector>

template <typename T>
concept RegistryEntry = requires(T entry) {
    { entry.displayName } -> std::convertible_to<std::string>;
    { entry.factory() };
};

template <typename EntryType> requires RegistryEntry<EntryType>
class Registry
{
protected:
    std::map<std::string, EntryType> m_regEntries {};

public:
    Registry() noexcept  = default;
    ~Registry() noexcept = default;

    void registerFactory(std::string const& t_id, EntryType t_entry)
    {
        m_regEntries[t_id] = std::move(t_entry);
    }

    EntryType* get(std::string const& t_id)
    {
        auto it = m_regEntries.find(t_id);
        if (it != m_regEntries.end()) { return &it->second; }

        return nullptr;
    }

    EntryType const* get(std::string const& t_id) const
    {
        auto it = m_regEntries.find(t_id);
        if (it != m_regEntries.end()) { return &it->second; }

        return nullptr;
    }
};
