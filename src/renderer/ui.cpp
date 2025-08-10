#include "renderer/ui.h"

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "core/platform/display.h"
#include "renderer/state.h"
#include "sort/category.h"
#include "sort/flags.h"
#include "sort/sort.h"
#include "utils/common.h"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <algorithm>
#include <format>
#include <limits>
#include <optional>
#include <thread>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace Renderer
{
    UI::UI(std::shared_ptr<Core::App> t_app) noexcept : m_app {t_app} { }

    void UI::renderText(std::string const& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            Core::Ctx const* const ctx         = appShared->getContext();

            SDL_Surface*           textSurface = TTF_RenderText_Solid(ctx->font, t_txt.c_str(), 0, t_col);
            SDL_Texture*           text        = SDL_CreateTextureFromSurface(ctx->renderer, textSurface);
            float                  text_width  = static_cast<float>(textSurface->w);
            float                  text_height = static_cast<float>(textSurface->h);

            SDL_DestroySurface(textSurface);
            SDL_FRect renderQuad {t_x, t_y, text_width, text_height};
            SDL_RenderTexture(ctx->renderer, text, nullptr, &renderQuad);
            SDL_DestroyTexture(text);
        }
    }

    void UI::renderInfo() const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            auto&                          sorter       = appShared->getSorter();
            Core::SortRegistry const&      registry     = appShared->getRegistry();
            Core::Ctx const* const         ctx          = appShared->getContext();

            Sort::Flags&                   flags        = sorter->getFlags();
            std::vector<std::string>       ids          = registry.idsByCategory(m_uiState.sortCategory);
            Core::SortRegistryEntry const* currentEntry = registry.get(ids[m_uiState.sortIndex]);

            Uint8                          _r, _g, _b, _a;
            SDL_GetRenderDrawColor(ctx->renderer, &_r, &_g, &_b, &_a);
            SDL_FRect rect {0.0f, 0.0f, 300.0f, 160.0f};
            SDL_SetRenderDrawColor(ctx->renderer, 0x40, 0x40, 0x40, 0x80);
            SDL_RenderFillRect(ctx->renderer, &rect);

            SDL_Color textColor {0, 0xFF, 0, 0};

            if (flags.isRunning || (sorter->timer.getDuration() == 0.0f))
            {
                textColor = {0xFF, 0xFF, 0xFF, 0};
            }

            renderText(
                "TIME: " + std::to_string(sorter->timer.getDuration()) + 's', 10.0f, 10.0f,
                textColor
            );
            renderText(
                "REAL TIME: " + std::to_string(sorter->realTimer.getDuration()) + 's', 10.0f, 30.0f,
                textColor
            );

            {
                std::string name {};
                if (m_uiState.sortIndex < ids.size()) { name = currentEntry->displayName; }
                renderText(std::string("SORT: ") + name, 10.0f, 50.0f, {0xFF, 0xFF, 0xFF, 0});
            }

            renderText(
                "SWAPS: " + std::to_string(sorter->elems.getSwaps()), 10.0f, 70.0f,
                {0xFF, 0xFF, 0xFF, 0}
            );
            renderText(
                "COMPARISONS: " + std::to_string(sorter->elems.getComparisons()), 10.0f, 90.0f,
                {0xFF, 0xFF, 0xFF, 0}
            );

            std::string statusText {"IDLE"};

            if (flags.isSorting) { statusText = "SORTING..."; }

            if (flags.isShuffling) { statusText = "SHUFFLING..."; }

            if (flags.isChecking) { statusText = "CHECKING..."; }

            if (flags.isSorted) { statusText = "SORTED!"; }

            if (flags.hasAborted) { statusText = "ABORTED!"; }
            renderText(statusText, 10.0f, 120.0f, {0xFF, 0xFF, 0xFF, 0});

            SDL_SetRenderDrawColor(ctx->renderer, _r, _g, _b, _a);
        }
    }

    void UI::renderDebugMenu()
    {
#define IMGUI_DEBUG_FLAG(flags, boolVal) ImGui::Text(std::format(#boolVal ": {}", flags.boolVal.load()).c_str());
#define IMGUI_DEBUG_UISTATE(uiObj, data) ImGui::Text(std::format(#data ": {}", uiObj.data).c_str());

        if (auto appShared = m_app.lock())
        {
            Sort::Flags& flags = appShared->getSorter()->getFlags();
            if (ImGui::CollapsingHeader("Sorting Flags"))
            {
                IMGUI_DEBUG_FLAG(flags, hasAborted);
                IMGUI_DEBUG_FLAG(flags, hasQuit);

                IMGUI_DEBUG_FLAG(flags, isChecking);
                IMGUI_DEBUG_FLAG(flags, isRunning);
                IMGUI_DEBUG_FLAG(flags, isShuffling);
                IMGUI_DEBUG_FLAG(flags, isSorted);
                IMGUI_DEBUG_FLAG(flags, isSorting);

                IMGUI_DEBUG_FLAG(flags, shouldSort);
            }
            ImGui::Separator();
            if (ImGui::CollapsingHeader("UI State"))
            {
                IMGUI_DEBUG_UISTATE(m_uiState, arrayLength);
                IMGUI_DEBUG_UISTATE(m_uiState, isColored);
                IMGUI_DEBUG_UISTATE(m_uiState, isReversed);
                IMGUI_DEBUG_UISTATE(m_uiState, sortParameter);
                IMGUI_DEBUG_UISTATE((int) m_uiState, sortCategory);
                IMGUI_DEBUG_UISTATE((int) m_uiState, sortDisplayType);
                IMGUI_DEBUG_UISTATE(m_uiState, sortIndex);
            }
        }
#undef IMGUI_DEBUG_BOOL
    }

    void UI::renderSortChooser(
        Core::SortRegistry const&       t_registry,
        std::string const&              t_currentName,
        std::vector<std::string> const& t_ids
    )
    {
        if (ImGui::BeginCombo(
                "##combo1", m_sortCategories[m_uiState.sortCategory]
            ))  // The second parameter is the label previewed before opening the combo.
        {
            for (std::size_t n = 0; n < m_sortCategories.size(); ++n)
            {
                bool isSelected
                    = (m_sortCategories[m_uiState.sortCategory]
                       == m_sortCategories[n]);  // You can store your selection
                                                 // however you want, outside or
                                                 // inside your objects
                if (ImGui::Selectable(m_sortCategories[n], isSelected))
                {
                    m_uiState.sortCategory = static_cast<Sort::Category>(n);
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();  // You may set the initial focus when
                                                   // opening the combo (scrolling + for
                                                   // keyboard navigation support)
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo(
                "##combo2", t_currentName.c_str()
            ))  // The second parameter is the label previewed before opening the combo.
        {
            for (std::size_t n = 0; n < t_ids.size(); ++n)
            {
                Core::SortRegistryEntry const* selectedEntry = t_registry.get(t_ids[n]);
                bool                           isSelected    = (n == m_uiState.sortIndex);

                if (ImGui::Selectable(selectedEntry->displayName.c_str(), isSelected))
                {
                    m_uiState.sortIndex = n;
                }
                if (isSelected) { ImGui::SetItemDefaultFocus(); }
            }
            ImGui::EndCombo();
        }
    }

    void UI::renderSortDisplayConfigs()
    {
        if (ImGui::BeginCombo(
                "##combo3", m_sortDisplayTypes[m_uiState.sortDisplayType]
            ))  // The second parameter is the label previewed before opening the combo.
        {
            for (std::size_t n = 0; n < m_sortDisplayTypes.size(); ++n)
            {
                bool isSelected
                    = (m_sortDisplayTypes[m_uiState.sortDisplayType]
                       == m_sortDisplayTypes[n]);  // You can store your selection
                                                   // however you want, outside or
                                                   // inside your objects

                if (ImGui::Selectable(m_sortDisplayTypes[n], isSelected))
                {
                    m_uiState.sortDisplayType = static_cast<DisplayType>(n);
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();  // You may set the initial focus when
                                                   // opening the combo (scrolling + for
                                                   // keyboard navigation support)
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Checkbox("Color", &m_uiState.isColored);
    }

    void UI::renderSortAlgorithmConfigs(Core::SortRegistryEntry const* const t_currentEntry)
    {
        ImGui::InputFloat("Set Speed", &Sort::BaseSort::s_speed, 0.001f);
        Sort::BaseSort::s_speed = std::clamp<float>(Sort::BaseSort::s_speed, 0.001f, 1000.f);

        int length              = static_cast<int>(m_uiState.arrayLength);
        ImGui::InputInt("Set Array Length", &length, 2);
        m_uiState.arrayLength = std::clamp<size_t>(length, 2, 1024 * 10);

        if (t_currentEntry->isParameterized)
        {
            static std::uint8_t                          prevIndex = std::numeric_limits<std::uint8_t>::max();

            static std::pair<std::int64_t, std::int64_t> bounds;

            if (m_uiState.sortIndex != prevIndex)
            {
                auto tempSorter        = t_currentEntry->factory();
                auto parameterizedSort = std::dynamic_pointer_cast<Sort::IParameterized>(tempSorter);
                bounds                 = parameterizedSort->getParameterBounds();

                prevIndex              = m_uiState.sortIndex;
            }

            m_uiState.sortParameter = std::clamp(m_uiState.sortParameter, bounds.first, bounds.second);

            ImGui::SliderScalar(
                "Set Buckets/Radix",
                ImGuiDataType_S64,
                &m_uiState.sortParameter,
                &bounds.first,
                &bounds.second,
                "%d"
            );
        }

        ImGui::SameLine();
        ImGui::Checkbox("Reverse instead of Shuffling", &m_uiState.isReversed);
    }

    void UI::renderSortButtons(
        Core::SortRegistryEntry const* const t_currentEntry,
        std::vector<std::string> const&      t_ids
    )
    {
        if (auto appShared = m_app.lock())
        {
            auto sorter = appShared->getSorter();

            if (!sorter->getFlags().isRunning)
            {
                if (ImGui::Button("Sort"))
                {
                    LOGINFO("Starting sort");
                    if (m_uiState.sortIndex < t_ids.size())
                    {
                        if (t_currentEntry)
                        {
                            appShared->setSorter(t_currentEntry->factory());

                            auto newSorter = appShared->getSorter();

                            newSorter->setLength(m_uiState.arrayLength);

                            if (t_currentEntry->isParameterized)
                            {
                                dynamic_pointer_cast<Sort::IParameterized>(newSorter)
                                    ->setParameter(
                                        m_uiState.sortParameter
                                    );
                            }

                            newSorter->getFlags().setFlags(Sort::FlagGroup::SortButtonPressed);
                        }
                    }
                    else { LOGERR("Unknown sort category/index"); }
                }
            }
            else
            {
                if (ImGui::Button("Stop"))
                {
                    sorter->getFlags().setFlags(Sort::FlagGroup::StopButtonPressed);

                    LOGINFO("Stopping sort");
                }
            }
        }
    }

    void UI::renderUI()
    {
        if (auto appShared = m_app.lock())
        {
            auto&                     sorter   = appShared->getSorter();
            Core::SortRegistry const& registry = appShared->getRegistry();
            Core::Ctx const*          ctx      = appShared->getContext();

            std::vector<std::string>  ids      = registry.idsByCategory(m_uiState.sortCategory);

            if (m_uiState.sortIndex >= ids.size()) { m_uiState.sortIndex = 0; }
            Core::SortRegistryEntry const* const currentEntry = registry.get(ids[m_uiState.sortIndex]);
            std::string const                    currentName  = currentEntry ? currentEntry->displayName : "";

            if (!m_uiState.isImGuiOpen)
            {
                sorter->getFlags().setFlags(Sort::FlagGroup::Quit);
            }

            renderInfo();

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();

            ImGui::NewFrame();

            // Main Window
            {
                ImGui::Begin("Configure", &m_uiState.isImGuiOpen);
                ImGui::Text(
                    "Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / Core::Platform::Display::getFramerate(),
                    Core::Platform::Display::getFramerate()
                );

                renderSortChooser(registry, currentName, ids);

                ImGui::Spacing();
                ImGui::SeparatorText("Display Config");
                ImGui::Spacing();

                renderSortDisplayConfigs();

                ImGui::Spacing();
                ImGui::SeparatorText("Variables");
                ImGui::Spacing();

                renderSortAlgorithmConfigs(currentEntry);

                ImGui::Spacing();

                renderSortButtons(currentEntry, ids);

                ImGui::End();
            }

            // Debug Window
            {
                ImGui::Begin("Debug");

                renderDebugMenu();

                ImGui::End();
            }

            ImGui::Render();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ctx->renderer);
        }
    }
}  // namespace Renderer
