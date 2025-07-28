#include "renderer/ui.h"

#include <atomic>
#include <algorithm>

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "renderer/state.h"
#include "utils/common.h"

namespace Renderer 
{
    void UI::renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(AppCtx::g_app->font, t_txt.c_str(), 0, t_col); // SDL_Surface* textSurface = TTF_RenderText_Solid(AppCtx::g_app->font, txt.c_str(), color);
        SDL_Texture* text = SDL_CreateTextureFromSurface(AppCtx::g_app->renderer, textSurface);
        float text_width = static_cast<float>(textSurface->w);
        float text_height = static_cast<float>(textSurface->h);
        SDL_DestroySurface(textSurface); // SDL_FreeSurface(textSurface);
        SDL_FRect renderQuad { t_x, t_y, text_width, text_height };
        SDL_RenderTexture(AppCtx::g_app->renderer, text, nullptr, &renderQuad); // SDL_RenderCopy(AppCtx::g_app->renderer, text, NULL, &renderQuad);
        SDL_DestroyTexture(text);
    }

    void UI::renderInfo() const noexcept
    {
        Uint8 _r, _g, _b, _a; SDL_GetRenderDrawColor(AppCtx::g_app->renderer, &_r, &_g, &_b, &_a);
        SDL_FRect rect { 0.0f, 0.0f, 300.0f, 160.0f };
        SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0x40, 0x40, 0x40, 0x80);
        SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);

        SDL_Color textColor { 0, 0xFF, 0, 0 };

        if (AppCtx::g_app->sorter->isSorting || (AppCtx::g_app->sorter->timer.getDuration() == 0.0f))
        {
            textColor = { 0xFF, 0xFF, 0xFF, 0 };
        }

        renderText("TIME: " + std::to_string(AppCtx::g_app->sorter->timer.getDuration()) + 's', 10.0f, 10.0f, textColor);
        renderText("REAL TIME: " + std::to_string(AppCtx::g_app->sorter->realTimer.getDuration()) + 's', 10.0f, 30.0f, textColor);
        
        {
            auto& reg = AppCtx::g_sortRegistry;
            auto ids = reg.idsByCategory(AppCtx::g_app->currentCategory);
            std::string name;
            if(AppCtx::g_app->currentItemIndex < ids.size()) 
            {
                if(auto* entry = reg.get(ids[AppCtx::g_app->currentItemIndex]))
                {
                    name = entry->displayName;
                }
            }
            renderText(std::string("SORT: ") + name, 10.0f, 50.0f, { 0xFF, 0xFF, 0xFF, 0 });
        }


        renderText("SWAPS: " + std::to_string(AppCtx::g_app->sorter->swaps), 10.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0 });
        renderText("COMPARISONS: " + std::to_string(AppCtx::g_app->sorter->comparisons), 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });


        std::string statusText{"IDLE"};

        if (AppCtx::g_app->sorter->isSorting)
        {
            statusText = "SORTING...";
        }

        if (AppCtx::g_app->sorter->isShuffling)
        {
            statusText = "SHUFFLING...";
        }

        if (AppCtx::g_app->sorter->isChecking)
        {
            statusText = "CHECKING...";
        }

        if (!(AppCtx::g_app->sorter->isShuffling) && !(AppCtx::g_app->sorter->isSorting) && !(AppCtx::g_app->sorter->isChecking) && AppCtx::g_app->sorter->sorted)
        {
            statusText = "SORTED!";
        }

        renderText(statusText, 10.0f, 130.0f, { 0xFF, 0xFF, 0xFF, 0 });

        SDL_SetRenderDrawColor(AppCtx::g_app->renderer, _r, _g, _b, _a);
    }

    Utils::Signal UI::renderUI()
    {
        static bool s_open = true;

        renderInfo();

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if(!s_open) 
        {
            AppCtx::g_app->sorter->wantClose = true;
            
            ImGui::Render();
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), AppCtx::g_app->renderer);
            return Utils::Signal::CloseApp;
        }

        std::atomic<bool> shouldSort = false;

        {
            auto& registry = AppCtx::g_sortRegistry;
            auto ids = registry.idsByCategory(AppCtx::g_app->currentCategory);
            if(AppCtx::g_app->currentItemIndex >= ids.size()) AppCtx::g_app->currentItemIndex = 0;
            const auto* currentEntry = registry.get(ids[AppCtx::g_app->currentItemIndex]);
            std::string currentName = currentEntry ? currentEntry->displayName : "";

            ImGui::Begin("Configure", &s_open);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / AppCtx::g_app->getFramerate(), AppCtx::g_app->getFramerate());
            
            if (ImGui::BeginCombo("##combo1", AppCtx::g_app->categories[AppCtx::g_app->currentCategory])) // The second parameter is the label previewed before opening the combo.
            {
                for (size_t n = 0; n < AppCtx::g_app->categories.size(); ++n)
                {
                    bool isSelected = (AppCtx::g_app->categories[AppCtx::g_app->currentCategory] == AppCtx::g_app->categories[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(AppCtx::g_app->categories[n], isSelected))
                    {
                        AppCtx::g_app->currentCategory = static_cast<Sort::Category>(n);
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
                    bool isSelected = (n == AppCtx::g_app->currentItemIndex);

                    if (ImGui::Selectable(entry->displayName.c_str(), isSelected))
                    {
                        AppCtx::g_app->currentItemIndex = n;
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

            if (ImGui::BeginCombo("##combo3", AppCtx::g_app->displayTypes[AppCtx::g_app->currentDisplayType])) // The second parameter is the label previewed before opening the combo.
            {
                for (size_t n = 0; n < AppCtx::g_app->displayTypes.size(); ++n)
                {
                    bool isSelected = (AppCtx::g_app->displayTypes[AppCtx::g_app->currentDisplayType] == AppCtx::g_app->displayTypes[n]); // You can store your selection however you want, outside or inside your objects
                    
                    if (ImGui::Selectable(AppCtx::g_app->displayTypes[n], isSelected))
                    {
                        AppCtx::g_app->currentDisplayType = static_cast<DisplayType>(n);
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

            ImGui::InputInt("Set Array Length", &Sort::BaseSort::s_length, 2);
            Sort::BaseSort::s_length = std::clamp(Sort::BaseSort::s_length, 2, 1024*10);

            if (currentName == "Radix LSD") 
            {
                ImGui::SliderInt("Set Buckets/Radix", &AppCtx::g_sortRadix, 2, 10, "%d");
            }

            ImGui::Spacing();
            if (!AppCtx::g_app->sorter->running)
            {
                if(ImGui::Button("Sort")) 
                {
                    LOGINFO("Starting sort");
                    auto& registry = AppCtx::g_sortRegistry;
                    auto ids = registry.idsByCategory(AppCtx::g_app->currentCategory);
                    if(AppCtx::g_app->currentItemIndex < ids.size()) 
                    {
                        auto* entry = registry.get(ids[AppCtx::g_app->currentItemIndex]);

                        if(entry)
                        {
                            AppCtx::g_app->sorter = entry->factory(AppCtx::g_app->data);
                            AppCtx::g_app->sorter->setLength(Sort::BaseSort::s_length);
                        }
                    } 
                    else 
                    {
                        LOGERR("Unknown sort category/index");
                    }

                    shouldSort = true;
                    AppCtx::g_app->sorter->running = true;
                }
            } 
            else 
            {
                if(ImGui::Button("Stop")) 
                {
                    AppCtx::g_app->sorter->running = false;
                    LOGINFO("Stopping sort");

                    AppCtx::g_app->sorter->timer.end();
                    AppCtx::g_app->sorter->realTimer.end();

                    AppCtx::g_app->sorter->wantStop = true;
                    if (AppCtx::g_app->sortThread.has_value()) 
                    {
                        if (AppCtx::g_app->sortThread->joinable())
                        {
                            AppCtx::g_app->sortThread->join();
                        }
                        
                        AppCtx::g_app->sortThread.reset();
                    }
                }
            }

            ImGui::SameLine();
            ImGui::Checkbox("Reverse instead of Shuffling", &State::reversed);
            ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), AppCtx::g_app->renderer);

        if(shouldSort)
        {
            if (AppCtx::g_app->sortThread.has_value()) 
            {
                if (AppCtx::g_app->sortThread->joinable())
                {
                    AppCtx::g_app->sortThread->join();
                }

                AppCtx::g_app->sortThread.reset();
            }

            AppCtx::g_app->sortThread = std::make_optional<std::thread>([&]() 
            {
                shouldSort = false;
                if(!State::reversed)
                {
                    AppCtx::g_app->sorter->shuffle();
                }
                else
                {
                    AppCtx::g_app->sorter->reverse();
                }
                
                if(!(AppCtx::g_app->sorter->wantStop)) 
                {
                    LOGINFO("Sorting");
                    AppCtx::g_app->sorter->timer.start();
                    AppCtx::g_app->sorter->realTimer.start();
                    AppCtx::g_app->sorter->sort();
                    AppCtx::g_app->sorter->realTimer.pause();
                }
                
                if(!(AppCtx::g_app->sorter->wantStop)) 
                {
                    LOGINFO("Checking");
                    AppCtx::g_app->sorter->realTimer.resume();
                    AppCtx::g_app->sorter->check();
                }

                if (AppCtx::g_app->sorter->wantStop) 
                {
                    return;
                }
                AppCtx::g_app->sorter->running = false;
            });
        }

        if (AppCtx::g_app->sorter->wantClose) 
        {
            return Utils::Signal::CloseApp;
        }

        if (AppCtx::g_app->sorter->wantStop) 
        {
            return Utils::Signal::StopSort;
        }
        
        return Utils::Signal::Success;
    }
}