#include "core/registry/sort_registry.h"

#include "core/logging/logging.h"
#include "renderer/context.h"
#include "sort/sort.h"

#include <format>

namespace Core
{
    SortRegistry::SortRegistry(std::shared_ptr<Core::App> t_app) : m_app {t_app} { }

    template <typename Factory>
    void SortRegistry::registerSort(
        std::string const& t_id,
        Sort::Category     t_category,
        std::string const& t_displayName,
        Factory            t_factory
    ) requires SortFactory<Factory>
    {
        using FactoryReturnType        = std::invoke_result_t<Factory>::element_type;

        constexpr bool isParameterized = std::is_base_of_v<Sort::IParameterized, FactoryReturnType>;

        LOG_INFO(
            std::format(
                "Registering sort:\n\tID: {}\n\tType: <{}{}>\n\tCategory: {}",
                t_id,
                Utils::demangleName(typeid(FactoryReturnType).name()),
                isParameterized ? ": Parameterized" : "",
                static_cast<int>(t_category)
            )
        );

        registerFactory(
            t_id, SortRegistryEntry {t_id, t_category, t_displayName, std::move(t_factory), isParameterized}
        );
    }

    std::vector<std::string> SortRegistry::idsByCategory(Sort::Category t_category) const
    {
        std::vector<std::string> ids;

        for (auto const& [id, entry] : m_regEntries)
        {
            if (entry.category == t_category) { ids.push_back(id); }
        }
        return ids;
    }

    void SortRegistry::registerAllSorts()
    {
        registerSort(
            "bubble", Sort::Category::Exchange, "Bubble Sort",
            []()
            { return std::make_shared<Sort::BubbleSort>(); }
        );
        registerSort(
            "quick", Sort::Category::Exchange, "Quick Sort",
            []()
            { return std::make_shared<Sort::QuickSort>(); }
        );
        registerSort(
            "comb", Sort::Category::Exchange, "Comb Sort",
            []()
            { return std::make_shared<Sort::CombSort>(); }
        );

        registerSort(
            "radix_lsd", Sort::Category::Distribution, "Radix LSD Sort",
            []()
            { return std::make_shared<Sort::RadixLSDSort>(); }
        );
        registerSort(
            "pigeon_hole", Sort::Category::Distribution, "Pigeon Hole Sort",
            []()
            { return std::make_shared<Sort::PigeonHoleSort>(); }
        );
        registerSort(
            "gravity", Sort::Category::Distribution, "Gravity Sort",
            []()
            { return std::make_shared<Sort::GravitySort>(); }
        );
        registerSort(
            "bogo", Sort::Category::Distribution, "Bogo Sort",
            []()
            { return std::make_shared<Sort::BogoSort>(); }
        );

        registerSort(
            "insertion", Sort::Category::Insertion, "Insertion Sort",
            []()
            { return std::make_shared<Sort::InsertionSort>(); }
        );

        registerSort(
            "merge", Sort::Category::Merge, "Merge Sort",
            []()
            { return std::make_shared<Sort::MergeSort>(); }
        );

        registerSort(
            "selection", Sort::Category::Select, "Selection Sort",
            []()
            { return std::make_shared<Sort::SelectionSort>(); }
        );
    }
}  // namespace Core
