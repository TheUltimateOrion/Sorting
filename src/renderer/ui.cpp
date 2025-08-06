#include "renderer/ui.h"

#include <algorithm>
#include <atomic>
#include <format>

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "core/platform/display.h"
#include "renderer/state.h"
#include "sort/category.h"
#include "sort/flags.h"
#include "sort/sort.h"
#include "utils/common.h"

namespace Renderer
{
    UI::UI(std::shared_ptr<Core::App> t_app) noexcept : m_app {t_app} { }

    void UI::renderText(std::string const& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            Core::Ctx*   ctx         = appShared->getContext();

            SDL_Surface* textSurface = TTF_RenderText_Solid(ctx->font, t_txt.c_str(), 0, t_col);
            SDL_Texture* text        = SDL_CreateTextureFromSurface(ctx->renderer, textSurface);
            float        text_width  = static_cast<float>(textSurface->w);
            float        text_height = static_cast<float>(textSurface->h);
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
            Core::Ctx*                     ctx          = appShared->getContext();

            // Sort::Flags & sorter->getFlags()            = sorter->getFlags();
            std::vector<std::string>       ids          = registry.idsByCategory(m_uiState.sortCategory);
            Core::SortRegistryEntry const* currentEntry = registry.get(ids[m_uiState.sortIndex]);

            Uint8                          _r, _g, _b, _a;
            SDL_GetRenderDrawColor(ctx->renderer, &_r, &_g, &_b, &_a);
            SDL_FRect rect {0.0f, 0.0f, 300.0f, 160.0f};
            SDL_SetRenderDrawColor(ctx->renderer, 0x40, 0x40, 0x40, 0x80);
            SDL_RenderFillRect(ctx->renderer, &rect);

            SDL_Color textColor {0, 0xFF, 0, 0};

            if (sorter->getFlags().isSorting || (sorter->timer.getDuration() == 0.0f))
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

            if (sorter->getFlags().isSorting) { statusText = "SORTING..."; }

            if (sorter->getFlags().isShuffling) { statusText = "SHUFFLING..."; }

            if (sorter->getFlags().isChecking) { statusText = "CHECKING..."; }

            if (!(sorter->getFlags().isShuffling) && !(sorter->getFlags().isSorting) && !(sorter->getFlags().isChecking)
                && sorter->getFlags().hasSorted)
            {
                statusText = "SORTED!";
            }

            renderText(statusText, 10.0f, 110.0f, {0xFF, 0xFF, 0xFF, 0});

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
            ImGui::Begin("Debug");
            if (ImGui::CollapsingHeader("Sorting Flags"))
            {
                IMGUI_DEBUG_FLAG(flags, hasRadix);
                IMGUI_DEBUG_FLAG(flags, hasSorted);
                IMGUI_DEBUG_FLAG(flags, isChecking);
                IMGUI_DEBUG_FLAG(flags, isRunning);
                IMGUI_DEBUG_FLAG(flags, isShuffling);
                IMGUI_DEBUG_FLAG(flags, isSorting);
                IMGUI_DEBUG_FLAG(flags, shouldSort);
                IMGUI_DEBUG_FLAG(flags, wantClose);
                IMGUI_DEBUG_FLAG(flags, wantStop);
            }
            ImGui::Separator();
            if (ImGui::CollapsingHeader("UI State"))
            {
                IMGUI_DEBUG_UISTATE(m_uiState, arrayLength);
                IMGUI_DEBUG_UISTATE(m_uiState, isColored);
                IMGUI_DEBUG_UISTATE(m_uiState, isReversed);
                IMGUI_DEBUG_UISTATE(m_uiState, radix);
                IMGUI_DEBUG_UISTATE((int) m_uiState, sortCategory);
                IMGUI_DEBUG_UISTATE((int) m_uiState, sortDisplayType);
                IMGUI_DEBUG_UISTATE(m_uiState, sortIndex);
            }
            ImGui::End();
        }
#undef IMGUI_DEBUG_BOOL
    }

    Utils::Signal UI::renderUI()
    {
        if (auto appShared = m_app.lock())
        {
            auto&                     sorter   = appShared->getSorter();
            Core::SortRegistry const& registry = appShared->getRegistry();
            Core::Ctx*                ctx      = appShared->getContext();

            // Sort::Flags&              sorter->getFlags() = sorter->getFlags();
            std::vector<std::string>  ids      = registry.idsByCategory(m_uiState.sortCategory);

            if (m_uiState.sortIndex >= ids.size()) { m_uiState.sortIndex = 0; }
            Core::SortRegistryEntry const* currentEntry = registry.get(ids[m_uiState.sortIndex]);
            std::string const              currentName  = currentEntry ? currentEntry->displayName : "";

            renderInfo();

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            {
                ImGui::Begin("Configure", &m_uiState.isImGuiOpen);
                ImGui::Text(
                    "Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / Core::Platform::Display::getFramerate(),
                    Core::Platform::Display::getFramerate()
                );

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
                        "##combo2", currentName.c_str()
                    ))  // The second parameter is the label previewed before opening the combo.
                {
                    for (std::size_t n = 0; n < ids.size(); ++n)
                    {
                        Core::SortRegistryEntry const* selectedEntry = registry.get(ids[n]);
                        bool                           isSelected    = (n == m_uiState.sortIndex);

                        if (ImGui::Selectable(selectedEntry->displayName.c_str(), isSelected))
                        {
                            m_uiState.sortIndex = n;
                        }
                        if (isSelected) { ImGui::SetItemDefaultFocus(); }
                    }
                    ImGui::EndCombo();
                }

                ImGui::Spacing();
                ImGui::SeparatorText("Display Config");
                ImGui::Spacing();

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

                ImGui::Spacing();
                ImGui::SeparatorText("Variables");
                ImGui::Spacing();

                ImGui::InputFloat("Set Speed", &Sort::BaseSort::s_speed, 0.001f);
                Sort::BaseSort::s_speed = std::clamp<float>(Sort::BaseSort::s_speed, 0.001f, 1000.f);

                int length              = static_cast<int>(m_uiState.arrayLength);
                ImGui::InputInt("Set Array Length", &length, 2);
                m_uiState.arrayLength = std::clamp<size_t>(length, 2, 1024 * 10);

                if (currentEntry->id == "radix_lsd")
                {
                    int radix = static_cast<int>(m_uiState.radix);
                    ImGui::SliderInt("Set Buckets/Radix", &radix, 2, 10, "%d");
                    m_uiState.radix = static_cast<std::uint8_t>(radix);
                }

                ImGui::Spacing();
                if (!sorter->getFlags().isRunning)
                {
                    if (ImGui::Button("Sort"))
                    {
                        LOGINFO("Starting sort");
                        if (m_uiState.sortIndex < ids.size())
                        {
                            if (currentEntry)
                            {
                                appShared->setSorter(currentEntry->factory());
                                sorter->setLength(m_uiState.arrayLength);

                                if (sorter->getFlags().hasRadix)
                                {
                                    dynamic_pointer_cast<Sort::RadixLSDSort>(sorter)->setRadix(
                                        m_uiState.radix
                                    );
                                }
                            }
                        }
                        else { LOGERR("Unknown sort category/index"); }

                        sorter->getFlags().setFlags(Sort::FlagGroup::SortButtonPressed);
                    }
                }
                else
                {
                    if (ImGui::Button("Stop"))
                    {
                        sorter->getFlags().setFlags(Sort::FlagGroup::StopButtonPressed);

                        LOGINFO("Stopping sort");

                        sorter->timer.end();
                        sorter->realTimer.end();

                        Utils::terminateThread(appShared->sortThread);
                    }
                }

                ImGui::SameLine();
                ImGui::Checkbox("Reverse instead of Shuffling", &m_uiState.isReversed);
                ImGui::End();

                renderDebugMenu();

                ImGui::Render();
                ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ctx->renderer);

                if (!m_uiState.isImGuiOpen)
                {
                    sorter->getFlags().setFlags(Sort::FlagGroup::Quit);

                    Utils::terminateThread(appShared->sortThread);

                    LOGINFO("Exit signal recieved");

                    return Utils::Signal::CloseApp;
                }

                if (sorter->getFlags().shouldSort)
                {
                    Utils::terminateThread(appShared->sortThread);

                    sorter->getFlags().shouldSort = false;
                    bool isReversed               = m_uiState.isReversed;

                    appShared->sortThread         = std::make_optional<std::thread>(
                        [appShared, isReversed]()
                        {
                            auto sorter = appShared->getSorter();
                            // Sort::Flags & sorter->getFlags() = sorter->getFlags();

                            if (!isReversed)
                            {
                                LOGINFO("Shuffling");

                                sorter->shuffle();
                            }
                            else
                            {
                                LOGINFO("Reversing");

                                sorter->reverse();
                            }

                            if (!sorter->getFlags().wantStop)
                            {
                                LOGINFO("Sorting");

                                sorter->timer.start();
                                sorter->realTimer.start();

                                sorter->getFlags().isSorting = true;
                                sorter->sort();
                                sorter->getFlags().setFlags(Sort::FlagGroup::DoneSorting);

                                sorter->realTimer.pause();
                                sorter->timer.pause();
                            }

                            if (!sorter->getFlags().wantStop)
                            {
                                LOGINFO("Checking");
                                sorter->check();
                            }

                            if (sorter->getFlags().wantStop) { return; }

                            sorter->getFlags().isRunning = false;
                        }
                    );
                }

                if (sorter->getFlags().wantClose) { return Utils::Signal::CloseApp; }

                if (sorter->getFlags().wantStop) { return Utils::Signal::StopSort; }

                return Utils::Signal::Success;
            }
        }

        return Utils::Signal::Success;
    }
}  // namespace Renderer
