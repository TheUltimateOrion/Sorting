#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "registry.h"
#include "sort_entry.h"
#include "sort/category.h"
#include "sort/base.h"

class SortRegistry : public Registry<SortRegistryEntry> {
public:
    void registerSort(
        const std::string& id,
        SortCategory category,
        const std::string& displayName,
        std::function<std::shared_ptr<BaseSort>(std::vector<int>&)> factory
    );

    std::vector<std::string> idsByCategory(SortCategory category) const;

    void registerAllSorts();
};
extern int g_sortRadix; // configurable radix for RadixLSDSort
