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
        std::function<std::shared_ptr<Sort::BaseSort>(std::vector<int>&)> factory
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
            [](std::vector<int>& arr){ return std::make_shared<Sort::BubbleSort>(arr); });
        registerSort("quick", Sort::Category::Exchange, "Quick Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::QuickSort>(arr); });
        registerSort("comb", Sort::Category::Exchange, "Comb Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::CombSort>(arr); });

        registerSort("radix_lsd", Sort::Category::Distribution, "Radix LSD Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::RadixLSDSort>(arr, 2); });
        registerSort("pigeon_hole", Sort::Category::Distribution, "Pigeon Hole Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::PigeonHoleSort>(arr); });
        registerSort("gravity", Sort::Category::Distribution, "Gravity Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::GravitySort>(arr); });
        registerSort("bogo", Sort::Category::Distribution, "Bogo Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::BogoSort>(arr); });

        registerSort("insertion", Sort::Category::Insertion, "Insertion Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::InsertionSort>(arr); });

        registerSort("merge", Sort::Category::Merge, "Merge Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::MergeSort>(arr); });

        registerSort("selection", Sort::Category::Select, "Selection Sort",
            [](std::vector<int>& arr){ return std::make_shared<Sort::SelectionSort>(arr); });
    }
}