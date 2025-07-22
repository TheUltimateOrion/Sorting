#include "core/registry/sort_registry.h"
#include "sort/sort.h"

#include "core/app_ctx.h"
#include "core/logging/logging.h"

void SortRegistry::registerSort(
    const std::string& id,
    SortCategory category,
    const std::string& displayName,
    std::function<std::shared_ptr<BaseSort>(std::vector<int>&)> factory
) {
    LOGINFO("Registering sort: " + id + " in category: " + std::to_string(static_cast<int>(category)));
    registerFactory(id, SortRegistryEntry{category, displayName, std::move(factory)});
}

std::vector<std::string> SortRegistry::idsByCategory(SortCategory category) const {
    std::vector<std::string> ids;
    for (const auto& [id, entry] : m_regEntries) {
        if (entry.category == category) ids.push_back(id);
    }
    return ids;
}

void SortRegistry::registerAllSorts() {
    auto& reg = AppCtx::g_sortRegistry;
    reg.registerSort("bubble", SortCategory::Exchange, "Bubble Sort",
        [](std::vector<int>& arr){ return std::make_shared<BubbleSort>(arr); });
    reg.registerSort("quick", SortCategory::Exchange, "Quick Sort",
        [](std::vector<int>& arr){ return std::make_shared<QuickSort>(arr); });
    reg.registerSort("comb", SortCategory::Exchange, "Comb Sort",
        [](std::vector<int>& arr){ return std::make_shared<CombSort>(arr); });

    reg.registerSort("radix_lsd", SortCategory::Distribution, "Radix LSD Sort",
        [](std::vector<int>& arr){ return std::make_shared<RadixLSDSort>(arr, AppCtx::g_sortRadix); });
    reg.registerSort("pigeon_hole", SortCategory::Distribution, "Pigeon Hole Sort",
        [](std::vector<int>& arr){ return std::make_shared<PigeonHoleSort>(arr); });
    reg.registerSort("gravity", SortCategory::Distribution, "Gravity Sort",
        [](std::vector<int>& arr){ return std::make_shared<GravitySort>(arr); });
    reg.registerSort("bogo", SortCategory::Distribution, "Bogo Sort",
        [](std::vector<int>& arr){ return std::make_shared<BogoSort>(arr); });

    reg.registerSort("insertion", SortCategory::Insertion, "Insertion Sort",
        [](std::vector<int>& arr){ return std::make_shared<InsertionSort>(arr); });

    reg.registerSort("merge", SortCategory::Merge, "Merge Sort",
        [](std::vector<int>& arr){ return std::make_shared<MergeSort>(arr); });

    reg.registerSort("selection", SortCategory::Select, "Selection Sort",
        [](std::vector<int>& arr){ return std::make_shared<SelectionSort>(arr); });
}