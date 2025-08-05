#include "renderer/ui.h"

#include <algorithm>
#include <atomic>

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "core/platform/display.h"
#include "renderer/state.h"
#include "sort/category.h"
#include "sort/sort.h"
#include "utils/common.h"

namespace Renderer
{
    UI::UI(std::shared_ptr<Core::App> t_app) noexcept : m_app(t_app) { };

    void UI::renderText(
        std::string const& t_txt,
        float              t_x,
        float              t_y,
        SDL_Color          t_col
    ) const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            SDL_Surface* textSurface = TTF_RenderText_Solid(
                appShared->font, t_txt.c_str(), 0, t_col
            );
            SDL_Texture* text        = SDL_CreateTextureFromSurface(appShared->ctx->renderer, textSurface);
            float        text_width  = static_cast<float>(textSurface->w);
            float        text_height = static_cast<float>(textSurface->h);
            SDL_DestroySurface(textSurface);
            SDL_FRect renderQuad {t_x, t_y, text_width, text_height};
            SDL_RenderTexture(appShared->ctx->renderer, text, nullptr, &renderQuad);
            SDL_DestroyTexture(text);
        }
    }

    void UI::renderInfo() const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            auto& sorter = appShared->getSorter();

            Uint8 _r, _g, _b, _a;
            SDL_GetRenderDrawColor(appShared->ctx->renderer, &_r, &_g, &_b, &_a);
            SDL_FRect rect {0.0f, 0.0f, 300.0f, 160.0f};
            SDL_SetRenderDrawColor(appShared->ctx->renderer, 0x40, 0x40, 0x40, 0x80);
            SDL_RenderFillRect(appShared->ctx->renderer, &rect);

            SDL_Color textColor {0, 0xFF, 0, 0};

            if (sorter->isSorting || (sorter->timer.getDuration() == 0.0f))
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
                auto&       reg = appShared->getRegistry();
                auto        ids = reg.idsByCategory(m_uiState.sortCategory);

                std::string name {};
                if (m_uiState.sortIndex < ids.size())
                {
                    if (auto* entry = reg.get(ids[m_uiState.sortIndex]))
                    {
                        name = entry->displayName;
                    }
                }
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

            if (sorter->isSorting) { statusText = "SORTING..."; }

            if (sorter->isShuffling) { statusText = "SHUFFLING..."; }

            if (sorter->isChecking) { statusText = "CHECKING..."; }

            if (!(sorter->isShuffling) && !(sorter->isSorting) && !(sorter->isChecking)
                && sorter->sorted)
            {
                statusText = "SORTED!";
            }

            renderText(statusText, 10.0f, 110.0f, {0xFF, 0xFF, 0xFF, 0});

            SDL_SetRenderDrawColor(appShared->ctx->renderer, _r, _g, _b, _a);
        }
    }

    Utils::Signal UI::renderUI()
    {
        static bool s_open = true;

        if (auto appShared = m_app.lock())
        {
            auto& sorter = appShared->getSorter();
            renderInfo();

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            if (!s_open)
            {
                sorter->wantClose = true;

                ImGui::Render();
                ImGui_ImplSDLRenderer3_RenderDrawData(
                    ImGui::GetDrawData(), appShared->ctx->renderer
                );
                return Utils::Signal::CloseApp;
            }

            std::atomic<bool> shouldSort = false;

            {
                if (auto appShared = m_app.lock())
                {
                    auto& registry = appShared->getRegistry();
                    auto  ids      = registry.idsByCategory(m_uiState.sortCategory);
                    if (m_uiState.sortIndex >= ids.size()) { m_uiState.sortIndex = 0; }
                    auto const* currentEntry = registry.get(ids[m_uiState.sortIndex]);
                    std::string currentName  = currentEntry ? currentEntry->displayName : "";

                    ImGui::Begin("Configure", &s_open);
                    ImGui::Text(
                        "Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / Core::Platform::Display::getFramerate(),
                        Core::Platform::Display::getFramerate()
                    );

                    if (ImGui::BeginCombo(
                            "##combo1", appShared->categories[m_uiState.sortCategory]
                        ))  // The second parameter is the label previewed before opening the combo.
                    {
                        for (std::size_t n = 0; n < appShared->categories.size(); ++n)
                        {
                            bool isSelected
                                = (appShared->categories[m_uiState.sortCategory]
                                   == appShared->categories[n]);  // You can store your selection
                                                                  // however you want, outside or
                                                                  // inside your objects
                            if (ImGui::Selectable(appShared->categories[n], isSelected))
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
                            auto const* entry      = registry.get(ids[n]);
                            bool        isSelected = (n == m_uiState.sortIndex);

                            if (ImGui::Selectable(entry->displayName.c_str(), isSelected))
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
                            "##combo3", appShared->displayTypes[appShared->currentDisplayType]
                        ))  // The second parameter is the label previewed before opening the combo.
                    {
                        for (std::size_t n = 0; n < appShared->displayTypes.size(); ++n)
                        {
                            bool isSelected
                                = (appShared->displayTypes[appShared->currentDisplayType]
                                   == appShared->displayTypes[n]);  // You can store your selection
                                                                    // however you want, outside or
                                                                    // inside your objects

                            if (ImGui::Selectable(appShared->displayTypes[n], isSelected))
                            {
                                appShared->currentDisplayType = static_cast<DisplayType>(n);
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
                    Sort::BaseSort::s_speed = std::clamp(Sort::BaseSort::s_speed, 0.001f, 1000.f);

                    int length              = static_cast<int>(m_uiState.arrayLength);
                    ImGui::InputInt("Set Array Length", &length, 2);
                    m_uiState.arrayLength = std::clamp(length, 2, 1024 * 10);

                    if (currentEntry->id == "radix_lsd")
                    {
                        int radix = static_cast<int>(m_uiState.radix);
                        ImGui::SliderInt("Set Buckets/Radix", &radix, 2, 10, "%d");
                        m_uiState.radix = static_cast<std::uint8_t>(radix);
                    }

                    ImGui::Spacing();
                    if (!sorter->running)
                    {
                        if (ImGui::Button("Sort"))
                        {
                            LOGINFO("Starting sort");
                            auto& registry = appShared->getRegistry();
                            auto  ids      = registry.idsByCategory(m_uiState.sortCategory);
                            if (m_uiState.sortIndex < ids.size())
                            {
                                auto* entry = registry.get(ids[m_uiState.sortIndex]);

                                if (entry)
                                {
                                    appShared->setSorter(entry->factory());
                                    sorter->setLength(m_uiState.arrayLength);

                                    if (sorter->hasRadix)
                                    {
                                        dynamic_pointer_cast<Sort::RadixLSDSort>(sorter)->setRadix(
                                            m_uiState.radix
                                        );
                                    }
                                }
                            }
                            else { LOGERR("Unknown sort category/index"); }

                            shouldSort      = true;
                            sorter->running = true;
                        }
                    }
                    else
                    {
                        if (ImGui::Button("Stop"))
                        {
                            sorter->running = false;
                            LOGINFO("Stopping sort");

                            sorter->timer.end();
                            sorter->realTimer.end();

                            sorter->wantStop = true;

                            Utils::terminateThread(appShared->sortThread);
                        }
                    }

                    ImGui::SameLine();
                    ImGui::Checkbox("Reverse instead of Shuffling", &m_uiState.isReversed);
                    ImGui::End();
                }

                ImGui::Render();

                ImGui_ImplSDLRenderer3_RenderDrawData(
                    ImGui::GetDrawData(), appShared->ctx->renderer
                );

                if (shouldSort)
                {
                    Utils::terminateThread(appShared->sortThread);

                    shouldSort            = false;
                    bool isReversed       = m_uiState.isReversed;

                    appShared->sortThread = std::make_optional<std::thread>(
                        [appShared, isReversed, &sorter]()
                        {
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

                            if (!(sorter->wantStop))
                            {
                                LOGINFO("Sorting");

                                sorter->timer.start();
                                sorter->realTimer.start();

                                sorter->sort();

                                sorter->realTimer.pause();
                                sorter->timer.pause();
                            }

                            if (!(sorter->wantStop))
                            {
                                LOGINFO("Checking");
                                sorter->check();
                            }

                            if (sorter->wantStop) { return; }

                            sorter->running = false;
                        }
                    );
                }

                if (sorter->wantClose) { return Utils::Signal::CloseApp; }

                if (sorter->wantStop) { return Utils::Signal::StopSort; }

                return Utils::Signal::Success;
            }
        }

        return Utils::Signal::Success;
    }
}  // namespace Renderer
