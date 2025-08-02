#include "core/registry/sort_registry.h"
#include "sort/sort.h"

#include "core/app_ctx.h"
#include "core/logging/logging.h"

namespace Core 
{
    SortRegistry::SortRegistry(std::shared_ptr<Core::App> app) : m_app(app) {}

    void SortRegistry::registerSort(
        const std::string& id,
        Sort::Category category,
        const std::string& displayName,
        std::function<std::shared_ptr<Sort::BaseSort>()> factory
    ) 
    {
        LOGINFO("Registering sort: " + id + " in category: " + std::to_string(static_cast<int>(category)));
        registerFactory(id, SortRegistryEntry{category, displayName, std::move(factory)});
    }

    std::vector<std::string> SortRegistry::idsByCategory(Sort::Category category) const 
    {
        std::vector<std::string> ids;
        for (const auto& [id, entry] : m_regEntries) 
        {
            if (entry.category == category) 
            {
                ids.push_back(id);
            }
        }
        return ids;
    }

    void SortRegistry::registerAllSorts() 
    {
        registerSort("bubble", Sort::Category::Exchange, "Bubble Sort",
            [](){ return std::make_shared<Sort::BubbleSort>(); });
        registerSort("quick", Sort::Category::Exchange, "Quick Sort",
            [](){ return std::make_shared<Sort::QuickSort>(); });
        registerSort("comb", Sort::Category::Exchange, "Comb Sort",
            [](){ return std::make_shared<Sort::CombSort>(); });

        registerSort("radix_lsd", Sort::Category::Distribution, "Radix LSD Sort",
            [](){ return std::make_shared<Sort::RadixLSDSort>(2); });
        registerSort("pigeon_hole", Sort::Category::Distribution, "Pigeon Hole Sort",
            [](){ return std::make_shared<Sort::PigeonHoleSort>(); });
        registerSort("gravity", Sort::Category::Distribution, "Gravity Sort",
            [](){ return std::make_shared<Sort::GravitySort>(); });
        registerSort("bogo", Sort::Category::Distribution, "Bogo Sort",
            [](){ return std::make_shared<Sort::BogoSort>(); });

        registerSort("insertion", Sort::Category::Insertion, "Insertion Sort",
            [](){ return std::make_shared<Sort::InsertionSort>(); });

        registerSort("merge", Sort::Category::Merge, "Merge Sort",
            [](){ return std::make_shared<Sort::MergeSort>(); });

        registerSort("selection", Sort::Category::Select, "Selection Sort",
            [](){ return std::make_shared<Sort::SelectionSort>(); });
    }
}