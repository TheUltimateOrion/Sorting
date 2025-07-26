#pragma once
#include <string>
#include <map>
#include <vector>

template <typename EntryType>
class Registry 
{
protected:
    std::map<std::string, EntryType> m_regEntries;
public:
    Registry() = default;
    ~Registry() = default;
    
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