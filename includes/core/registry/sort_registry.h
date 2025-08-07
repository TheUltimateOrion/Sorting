#pragma once

#include "registry.h"
#include "sort/base.h"
#include "sort/category.h"
#include "sort/parametrized.h"
#include "sort_entry.h"

#include <concepts>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace Core
{
    class App;

    template <typename Factory>
    concept SortFactory
        = std::invocable<Factory>
       && std::same_as<
              std::invoke_result_t<Factory>,
              std::shared_ptr<typename std::invoke_result_t<Factory>::element_type>>
       && std::derived_from<typename std::invoke_result_t<Factory>::element_type, Sort::BaseSort>;

    class SortRegistry : public Registry<SortRegistryEntry>
    {
    private:
        std::weak_ptr<Core::App> m_app;

    public:
        SortRegistry(std::shared_ptr<Core::App> t_app);

        template <typename Factory>
        void registerSort(
            std::string const& t_id,
            Sort::Category     t_category,
            std::string const& t_displayName,
            Factory            t_factory
        ) requires SortFactory<Factory>;

        std::vector<std::string> idsByCategory(Sort::Category t_category) const;

        void                     registerAllSorts();
    };
}  // namespace Core
