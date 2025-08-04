#pragma once

#include <string>
#include <map>
#include <vector>
#include <concepts>

template <class T>
concept RegistryEntry = requires(T entry)
{
    { entry.displayName } -> std::convertible_to<std::string>;
    { entry.factory() };
};

template <typename EntryType>
requires RegistryEntry<EntryType>
class Registry 
{
protected:
    std::map<std::string, EntryType> m_regEntries;
public:
    Registry() noexcept = default;
    ~Registry() noexcept = default;
    
    void registerFactory(const std::string& id, EntryType entry)
    {
        m_regEntries[id] = std::move(entry);
    }

    EntryType* get(const std::string& id)
    {
        auto it = m_regEntries.find(id);
        if (it != m_regEntries.end())
        {
            return &it->second;
        }
        
        return nullptr;
    }
    
    const EntryType* get(const std::string& id) const
    {
        auto it = m_regEntries.find(id);
        if (it != m_regEntries.end())
        {
            return &it->second;
        }
        
        return nullptr;
    }
};
