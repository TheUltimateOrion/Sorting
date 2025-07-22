#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "sort/category.h"
#include "sort/base.h"

struct SortRegistryEntry {
    SortCategory category;
    std::string displayName;
    std::function<std::shared_ptr<BaseSort>(std::vector<int>&)> factory;
};