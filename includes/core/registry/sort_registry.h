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
    class App;

    template<class Factory>
    concept SortFactory =   std::invocable<Factory> 
                        &&  std::same_as<std::invoke_result_t<Factory>, 
                                         std::shared_ptr<typename std::invoke_result_t<Factory>::element_type>> 
                        &&  std::derived_from<typename std::invoke_result_t<Factory>::element_type, Sort::BaseSort>;

    class SortRegistry : public Registry<SortRegistryEntry> 
    {
    private:
        std::weak_ptr<Core::App> m_app;
    public:
        SortRegistry(std::shared_ptr<Core::App> app);

        template<class Factory>
        void registerSort (
            const std::string& id,
            Sort::Category category,
            const std::string& displayName,
            Factory factory
        ) requires SortFactory<Factory>;

        std::vector<std::string> idsByCategory(Sort::Category category) const;

        void registerAllSorts();
    };
}
