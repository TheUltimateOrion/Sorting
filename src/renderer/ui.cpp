#include "renderer/ui.h"

#include <atomic>
#include <algorithm>

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "core/platform/display.h"
#include "renderer/state.h"
#include "utils/common.h"

namespace Renderer 
{
    UI::UI(std::shared_ptr<Core::App> app) noexcept : m_app(app), m_arrayLength(512) {};

    void UI::renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            SDL_Surface* textSurface = TTF_RenderText_Solid(appShared->font, t_txt.c_str(), 0, t_col); 
            SDL_Texture* text = SDL_CreateTextureFromSurface(appShared->ctx->renderer, textSurface);
            float text_width = static_cast<float>(textSurface->w);
            float text_height = static_cast<float>(textSurface->h);
            SDL_DestroySurface(textSurface);
            SDL_FRect renderQuad { t_x, t_y, text_width, text_height };
            SDL_RenderTexture(appShared->ctx->renderer, text, nullptr, &renderQuad);
            SDL_DestroyTexture(text);

        }
    }

    void UI::renderInfo() const noexcept
    {
        if (auto appShared = m_app.lock())
        {
            Uint8 _r, _g, _b, _a; SDL_GetRenderDrawColor(appShared->ctx->renderer, &_r, &_g, &_b, &_a);
            SDL_FRect rect { 0.0f, 0.0f, 300.0f, 160.0f };
            SDL_SetRenderDrawColor(appShared->ctx->renderer, 0x40, 0x40, 0x40, 0x80);
            SDL_RenderFillRect(appShared->ctx->renderer, &rect);

            SDL_Color textColor { 0, 0xFF, 0, 0 };

            if (appShared->sorter->isSorting || (appShared->sorter->timer.getDuration() == 0.0f))
            {
                textColor = { 0xFF, 0xFF, 0xFF, 0 };
            }

            renderText("TIME: " + std::to_string(appShared->sorter->timer.getDuration()) + 's', 10.0f, 10.0f, textColor);
            renderText("REAL TIME: " + std::to_string(appShared->sorter->realTimer.getDuration()) + 's', 10.0f, 30.0f, textColor);

            {
                auto& reg = appShared->sortRegistry;
                auto ids = reg.idsByCategory(appShared->currentCategory);
                std::string name;
                if(m_currentSortIndex < ids.size()) 
                {
                    if(auto* entry = reg.get(ids[m_currentSortIndex]))
                    {
                        name = entry->displayName;
                    }
                }
                renderText(std::string("SORT: ") + name, 10.0f, 50.0f, { 0xFF, 0xFF, 0xFF, 0 });
            }


            renderText("SWAPS: " + std::to_string(appShared->sorter->elems.getSwaps()), 10.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0 });
            renderText("COMPARISONS: " + std::to_string(appShared->sorter->elems.getComparisons()), 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });

            std::string statusText{"IDLE"};

            if (appShared->sorter->isSorting)
            {
                statusText = "SORTING...";
            }

            if (appShared->sorter->isShuffling)
            {
                statusText = "SHUFFLING...";
            }

            if (appShared->sorter->isChecking)
            {
                statusText = "CHECKING...";
            }

            if (!(appShared->sorter->isShuffling) && !(appShared->sorter->isSorting) && !(appShared->sorter->isChecking) && appShared->sorter->sorted)
            {
                statusText = "SORTED!";
            }

            renderText(statusText, 10.0f, 110.0f, { 0xFF, 0xFF, 0xFF, 0 });

            SDL_SetRenderDrawColor(appShared->ctx->renderer, _r, _g, _b, _a);
        }
    }

    Utils::Signal UI::renderUI()
    {
        static bool s_open = true;

        if (auto appShared = m_app.lock())
        {
            renderInfo();

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            if(!s_open) 
            {
                appShared->sorter->wantClose = true;
                
                ImGui::Render();
                ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), appShared->ctx->renderer);
                return Utils::Signal::CloseApp;
            }

            std::atomic<bool> shouldSort = false;

            {
                if (auto appShared = m_app.lock())
                {
                    auto& registry = appShared->sortRegistry;
                    auto ids = registry.idsByCategory(appShared->currentCategory);
                    if(m_currentSortIndex >= ids.size()) m_currentSortIndex = 0;
                    const auto* currentEntry = registry.get(ids[m_currentSortIndex]);
                    std::string currentName = currentEntry ? currentEntry->displayName : "";

                    ImGui::Begin("Configure", &s_open);
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Core::Platform::Display::getFramerate(), Core::Platform::Display::getFramerate());
                    
                    if (ImGui::BeginCombo("##combo1", appShared->categories[appShared->currentCategory])) // The second parameter is the label previewed before opening the combo.
                    {
                        for (size_t n = 0; n < appShared->categories.size(); ++n)
                        {
                            bool isSelected = (appShared->categories[appShared->currentCategory] == appShared->categories[n]); // You can store your selection however you want, outside or inside your objects
                            if (ImGui::Selectable(appShared->categories[n], isSelected))
                            {
                                appShared->currentCategory = static_cast<Sort::Category>(n);
                            }
                            if (isSelected)
                            {
                                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                            }
                        }
                        ImGui::EndCombo();
                    }

                    
                    if (ImGui::BeginCombo("##combo2", currentName.c_str())) // The second parameter is the label previewed before opening the combo.
                    {
                        for (size_t n = 0; n < ids.size(); ++n)
                        {
                            const auto* entry = registry.get(ids[n]);
                            bool isSelected = (n == m_currentSortIndex);

                            if (ImGui::Selectable(entry->displayName.c_str(), isSelected))
                            {
                                m_currentSortIndex = n;
                            }
                            if (isSelected) 
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Spacing();
                    ImGui::SeparatorText("Display Config");
                    ImGui::Spacing();

                    if (ImGui::BeginCombo("##combo3", appShared->displayTypes[appShared->currentDisplayType])) // The second parameter is the label previewed before opening the combo.
                    {
                        for (size_t n = 0; n < appShared->displayTypes.size(); ++n)
                        {
                            bool isSelected = (appShared->displayTypes[appShared->currentDisplayType] == appShared->displayTypes[n]); // You can store your selection however you want, outside or inside your objects
                            
                            if (ImGui::Selectable(appShared->displayTypes[n], isSelected))
                            {
                                appShared->currentDisplayType = static_cast<DisplayType>(n);
                            }
                            
                            if (isSelected)
                            {
                                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::SameLine();
                    ImGui::Checkbox("Color", &State::isColored);

                    ImGui::Spacing();
                    ImGui::SeparatorText("Variables");
                    ImGui::Spacing();

                    ImGui::InputFloat("Set Speed", &Sort::BaseSort::s_speed, 0.001f);
                    Sort::BaseSort::s_speed = std::clamp(Sort::BaseSort::s_speed, 0.001f, 1000.f);

                    
                    ImGui::InputInt("Set Array Length", &m_arrayLength, 2);
                    m_arrayLength = std::clamp(m_arrayLength, 2, 1024*10);

                    if (currentName == "Radix LSD") 
                    {
                        ImGui::SliderInt("Set Buckets/Radix", &appShared->sortRadix, 2, 10, "%d");
                    }

                    ImGui::Spacing();
                    if (!appShared->sorter->running)
                    {
                        if(ImGui::Button("Sort")) 
                        {
                            LOGINFO("Starting sort");
                            auto& registry = appShared->sortRegistry;
                            auto ids = registry.idsByCategory(appShared->currentCategory);
                            if(m_currentSortIndex < ids.size()) 
                            {
                                auto* entry = registry.get(ids[m_currentSortIndex]);

                                if(entry)
                                {
                                    appShared->sorter = entry->factory();
                                    appShared->sorter->setLength(m_arrayLength);
                                }
                            } 
                            else 
                            {
                                LOGERR("Unknown sort category/index");
                            }

                            shouldSort = true;
                            appShared->sorter->running = true;
                        }
                    } 
                    else 
                    {
                        if(ImGui::Button("Stop")) 
                        {
                            appShared->sorter->running = false;
                            LOGINFO("Stopping sort");

                            appShared->sorter->timer.end();
                            appShared->sorter->realTimer.end();

                            appShared->sorter->wantStop = true;
                            
                            Utils::terminateThread(appShared->sortThread);
                        }
                    }

                    ImGui::SameLine();
                    ImGui::Checkbox("Reverse instead of Shuffling", &State::reversed);
                    ImGui::End();
                }
                
                ImGui::Render();

                ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), appShared->ctx->renderer);
                
                if(shouldSort)
                {
                    Utils::terminateThread(appShared->sortThread);

                    shouldSort = false;
                    appShared->sortThread = std::make_optional<std::thread>([appShared]() 
                    {
                        if(!State::reversed)
                        {
                            LOGINFO("Shuffling");

                            appShared->sorter->shuffle();
                        }
                        else
                        {
                            LOGINFO("Reversing");

                            appShared->sorter->reverse();
                        }
                        
                        if(!(appShared->sorter->wantStop)) 
                        {
                            LOGINFO("Sorting");

                            appShared->sorter->timer.start();
                            appShared->sorter->realTimer.start();
                            
                            appShared->sorter->sort();
                            
                            appShared->sorter->realTimer.pause();
                            appShared->sorter->timer.pause();
                        }
                        
                        if(!(appShared->sorter->wantStop)) 
                        {
                            LOGINFO("Checking");
                            appShared->sorter->check();
                        }

                        if (appShared->sorter->wantStop) 
                        {
                            return;
                        }
                        
                        appShared->sorter->running = false;
                    });
                }

                if (appShared->sorter->wantClose) 
                {
                    return Utils::Signal::CloseApp;
                }

                if (appShared->sorter->wantStop) 
                {
                    return Utils::Signal::StopSort;
                }
                
                return Utils::Signal::Success;
            }
        }

        return Utils::Signal::Success;
    }
}