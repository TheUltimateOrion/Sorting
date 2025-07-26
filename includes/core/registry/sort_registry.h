#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "registry.h"
#include "sort_entry.h"
#include "sort/category.h"
#include "sort/base.h"

namespace Core 
{
    class SortRegistry : public Registry<SortRegistryEntry> 
    {
    public:
        void registerSort (
            const std::string& id,
            Sort::Category category,
            const std::string& displayName,
            std::function<std::shared_ptr<Sort::BaseSort>(std::vector<int>&)> factory
        );

        std::vector<std::string> idsByCategory(Sort::Category category) const;

        void registerAllSorts();
    };
}
