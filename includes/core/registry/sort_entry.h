#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "sort/base.h"
#include "sort/category.h"

namespace Core
{
    struct SortRegistryEntry
    {
        std::string                                      id;
        Sort::Category                                   category;
        std::string                                      displayName;
        std::function<std::shared_ptr<Sort::BaseSort>()> factory;
    };
}  // namespace Core
