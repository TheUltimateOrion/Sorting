#pragma once

#include "sort/base.h"
#include "sort/category.h"
#include "sort/parametrized.h"

#include <functional>
#include <memory>
#include <string>

namespace Core
{
    struct SortRegistryEntry
    {
        std::string                                      id {"unknown_id"};
        Sort::Category                                   category {Sort::Category::Unknown};
        std::string                                      displayName {"Unknown Sort"};
        std::function<std::shared_ptr<Sort::BaseSort>()> factory;

        bool                                             isParameterized;
    };
}  // namespace Core
