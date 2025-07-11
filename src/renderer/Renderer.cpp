#include "core/App.h"
#include "renderer/Renderer.h"
#include "sort/BubbleSort.h"
#include "sort/SelectionSort.h"
#include "sort/GravitySort.h"
#include "sort/PigeonHoleSort.h"
#include "sort/InsertionSort.h"
#include "sort/QuickSort.h"
#include "sort/RadixLSDSort.h"
#include "sort/CombSort.h"
#include "sort/BogoSort.h"
#include "sort/MergeSort.h"
#include "sort/SortCategories.h"
#include "renderer/DisplayType.h"

SDL_Color SortRenderer::HSVToRGB(unsigned char t_hue, unsigned char t_saturation, unsigned char t_value) const noexcept
{
    SDL_Color rgb;
    unsigned char region, remainder, p, q, t;
    region = t_hue / 43;
    remainder = (t_hue - (region * 43)) * 6; 
    
    p = (t_value * (0xFF - t_saturation)) >> 8;
    q = (t_value * (0xFF - ((t_saturation * remainder) >> 8))) >> 8;
    t = (t_value * (0xFF - ((t_saturation * (0xFF - remainder)) >> 8))) >> 8;
    
    switch (region)
    {
        case 0:
            rgb.r = t_value; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = t_value; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = t_value; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = t_value;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = t_value;
            break;
        default:
            rgb.r = t_value; rgb.g = p; rgb.b = q;
            break;
    }
    
    return rgb;
}

void SortRenderer::renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept
{
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(AppCtx::g_app->font, t_txt.c_str(), 0, t_col); // SDL_Surface* textSurface = TTF_RenderText_Solid(AppCtx::g_app->font, txt.c_str(), color);
    SDL_Texture* text = SDL_CreateTextureFromSurface(AppCtx::g_app->renderer, textSurface);
    float text_width = static_cast<float>(textSurface->w);
    float text_height = static_cast<float>(textSurface->h);
    SDL_DestroySurface(textSurface); // SDL_FreeSurface(textSurface);
    SDL_FRect renderQuad = { t_x, t_y, text_width, text_height};
    SDL_RenderTexture(AppCtx::g_app->renderer, text, nullptr, &renderQuad); // SDL_RenderCopy(AppCtx::g_app->renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}

void SortRenderer::renderInfo() const noexcept
{
    Uint8 _r, _g, _b, _a; SDL_GetRenderDrawColor(AppCtx::g_app->renderer, &_r, &_g, &_b, &_a);
    SDL_FRect rect = {0.0f, 0.0f, 230.0f, 125.0f};
    SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0x40, 0x40, 0x40, 0x80);
    SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);

    SDL_Color textColor = { 0, 0xFF, 0, 0 };
    if (AppCtx::g_app->sorter->isSorting)
        AppCtx::g_app->sorter->lastTime = AppCtx::getTimestamp() - AppCtx::g_app->sorter->startTime;
    if (AppCtx::g_app->sorter->isSorting || (AppCtx::g_app->sorter->lastTime == 0.0f))
        textColor = { 0xFF, 0xFF, 0xFF, 0 };
    
    renderText("TIME: " + std::to_string(AppCtx::g_app->sorter->lastTime) + 's', 10.0f, 10.0f, textColor);
    renderText(std::string("Sort: ") + AppCtx::g_app->items[AppCtx::g_app->currentCategory][AppCtx::g_app->current_item], 10, 30, { 0xFF, 0xFF, 0xFF, 0 });

    renderText("Swaps: " + std::to_string(AppCtx::g_app->sorter->swaps), 10.0f, 50.0f, { 0xFF, 0xFF, 0xFF, 0 });
    renderText("Comparisions: " + std::to_string(AppCtx::g_app->sorter->comparisions), 10.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0 });

    if (AppCtx::g_app->sorter->isSorting)
        renderText("Sorting...", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    if (AppCtx::g_app->sorter->isShuffling)
        renderText("Shuffling...", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    if (!(AppCtx::g_app->sorter->isShuffling) && !(AppCtx::g_app->sorter->isSorting) && AppCtx::g_app->sorter->sorted)
        renderText("Sorted", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    SDL_SetRenderDrawColor(AppCtx::g_app->renderer, _r, _g, _b, _a);
}

void SortRenderer::update() noexcept
{
    {
        LOCK_GUARD;
        m_elems = AppCtx::g_app->data;
    }
    SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(AppCtx::g_app->renderer);

    AppCtx::g_app->currentElement = AppCtx::g_app->sorter->getFirst();

    SDL_Color sortColor; 
    for (size_t k = 0; k < m_elems.size(); ++k)
    {
        sortColor = HSVToRGB(m_elems[k] * 0xFF / m_elems.size(), 0xFF, 0xFF);

        if (k == AppCtx::g_app->sorter->getFirst() || k == AppCtx::g_app->sorter->getSecond())
            SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0xFF, 0x0, 0x0, 0xFF);
        else {
            SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            if (AppCtx::g_app->isColored) {
                //SDL_SetRenderDrawColor(renderer, m_elems[k] * 255 / (m_elems.size()), 0, 255-(m_elems[k] * 255 / (m_elems.size())), 255);
                SDL_SetRenderDrawColor(AppCtx::g_app->renderer, sortColor.r, sortColor.g, sortColor.b, 0xFF);
            }
        }

        const float spacing = AppCtx::kWinWidth / m_elems.size();
        float degreesQuotient = 360.0f / static_cast<float>(m_elems.size());
        float radiansQuotient = (M_PI / 180);
        float size = 0.0f;
        
        switch (AppCtx::g_app->currentDisplayType) {
            case DisplayType::Bar: {
                // SDL_RenderDrawLine(renderer, k + 1, m_elems.size() , k + 1, m_elems.size()  - m_elems[k]);
                // std::cout << -m_elems[k] * spacing << std::endl;
                SDL_FRect rect = (SDL_FRect) {k * spacing, AppCtx::kWinHeight, spacing, -(m_elems[k] * spacing * AppCtx::kWinHeight / AppCtx::kWinWidth)};
                SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);
            } break;

            case DisplayType::Dot: {
                // SDL_RenderDrawPoint(renderer, k + 1, m_elems.size()  - m_elems[k]);
                SDL_RenderPoint(AppCtx::g_app->renderer, (k + 1) * spacing, (AppCtx::kWinHeight - m_elems[k] * spacing * AppCtx::kWinHeight / AppCtx::kWinWidth)); // SDL_RenderDrawPoint(AppCtx::g_app->renderer, (k + 1) * spacing, (m_elems.size() - m_elems[k]) * spacing);
            } break;

            case DisplayType::RainbowRectangle: {
                AppCtx::g_app->isColored = true;
                SDL_FRect rect = (SDL_FRect){k * spacing, 0, spacing, AppCtx::kWinHeight};
                SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);
            } break;

            case DisplayType::Circle: {
                AppCtx::g_app->isColored = true;
                float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
                SDL_GetRenderDrawColorFloat(AppCtx::g_app->renderer, &r, &g, &b, &a);

                size = std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) * 0.75 * 0.5;
                
                SDL_Vertex vertices[3];

                vertices[0] = {
                    {AppCtx::kWinWidth / 2, AppCtx::kWinHeight / 2}, /* first point location */ 
                    { r, g, b, 0xFF }, /* first color */ 
                    { 0.f, 0.f }
                };

                vertices[1] = {
                    {AppCtx::kWinWidth / 2 + size * cosf(radiansQuotient * k * degreesQuotient), AppCtx::kWinHeight / 2 + size * sinf(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                    { r, g, b, 0xFF }, /* second color */
                    { 0.f, 0.f }
                };

                vertices[2] = {
                    {AppCtx::kWinWidth / 2 + size * cosf(radiansQuotient * (k + 1) * degreesQuotient), AppCtx::kWinHeight / 2 + size * sinf(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                    { r, g, b, 0xFF }, /* third color */
                    { 0.f, 0.f }
                };

                if (!SDL_RenderGeometry(AppCtx::g_app->renderer, nullptr, vertices, 3, nullptr, 0))
                {
                    LOGERR(SDL_GetError());
                }
            } break;

            case DisplayType::CircleDot: {
                //std::cout << m_elems[k] / (k + 1) << std::endl;
                // size = 200.0f * ((float)m_elems[k] / (float)(k + 1));
                size = std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) * 0.75 * 0.5;
                SDL_RenderPoint(AppCtx::g_app->renderer, AppCtx::kWinWidth / 2 + size * cosf(radiansQuotient * k * degreesQuotient), AppCtx::kWinHeight / 2 + size * sinf(radiansQuotient * k * degreesQuotient));
            } break;

            case DisplayType::DisparityCircle: {
                size = std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) * 0.75 * 0.5;
                SDL_RenderLine(AppCtx::g_app->renderer, AppCtx::kWinWidth / 2 + size * cosf(radiansQuotient * m_elems[k] * degreesQuotient), AppCtx::kWinHeight / 2 + size * sinf(radiansQuotient * m_elems[k] * degreesQuotient), AppCtx::kWinWidth / 2 + size * cosf(radiansQuotient * (k + 1) * degreesQuotient), AppCtx::kWinHeight / 2 + size * sinf(radiansQuotient * (k + 1) * degreesQuotient));
            } break;

            case DisplayType::Spiral: {
                float r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColorFloat(AppCtx::g_app->renderer, &r, &g, &b, &a);
                
                size = 0.5f * std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) / static_cast<float>(m_elems.size());
                
                SDL_Vertex vertices[3];
                
                vertices[0] = {
                    {AppCtx::kWinWidth / 2, AppCtx::kWinHeight / 2}, /* first point location */ 
                    { r, g, b, 0xFF }, /* first color */ 
                    { 0.f, 0.f }
                };

                vertices[1] = {
                    {AppCtx::kWinWidth / 2 + size * m_elems[k] * cosf(radiansQuotient * k * degreesQuotient), AppCtx::kWinHeight / 2 + size * m_elems[k] * sinf(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                    { r, g, b, 0xFF }, /* second color */
                    { 0.f, 0.f }
                };

                vertices[2] = {
                    {AppCtx::kWinWidth / 2 + size * m_elems[k] * cosf(radiansQuotient * (k + 1) * degreesQuotient), AppCtx::kWinHeight / 2 + size * m_elems[k] * sinf(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                    { r, g, b, 0xFF }, /* third color */
                    { 0.f, 0.f }
                };

                // SDL_RenderDrawPoint(renderer, AppCtx::kWinWidth / 2 + 50 * (k / m_elems[k]) * cos(radiansQuotient * (k + 1) * degreesQuotient), AppCtx::kWinWidth / 2 + 50 * (k / m_elems[k]) * sin(radiansQuotient * (k + 1) * degreesQuotient));
                SDL_RenderGeometry(AppCtx::g_app->renderer, nullptr, vertices, 3, nullptr, 0);
            } break;

            case DisplayType::SpiralDot: {
                size = 0.5f * std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) / static_cast<float>(m_elems.size());
                SDL_RenderPoint(AppCtx::g_app->renderer, AppCtx::kWinWidth / 2 + size * m_elems[k] * cos(radiansQuotient * k * degreesQuotient), AppCtx::kWinHeight / 2 + size * m_elems[k] * sin(radiansQuotient * k * degreesQuotient)); // SDL_RenderDrawPoint(AppCtx::g_app->renderer, AppCtx::kWinWidth / 2 + size * m_elems[k] * cos(radiansQuotient * k * degreesQuotient), AppCtx::kWinWidth / 2 + size * m_elems[k] * sin(radiansQuotient * k * degreesQuotient));
            } break;
        }
    }

    renderInfo();

    int ret = renderGUI();
    if (ret == 1) {
        AppCtx::g_app->sorter->isShuffling = false;
        AppCtx::g_app->sorter->isSorting = false;
        AppCtx::g_app->sorter->sorted = true;
        AppCtx::g_app->sorter->wantStop = false;
        return;
    } else if (ret == 2) return;
        
    SDL_RenderPresent(AppCtx::g_app->renderer);
    if(SDL_PollEvent(&AppCtx::g_app->event))
    {
        ImGui_ImplSDL3_ProcessEvent(&AppCtx::g_app->event);
        if (AppCtx::g_app->event.type == SDL_EVENT_QUIT)
        {
            AppCtx::g_app->sorter->wantClose = true;
            return;
        }
    }

    std::this_thread::sleep_for(1ms);
    
}


int SortRenderer::renderGUI() const noexcept
{
    static bool s_open = true;

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if(!s_open) {
        AppCtx::g_app->sorter->wantClose = true;
        
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), AppCtx::g_app->renderer);
        return 2;
    }

    std::atomic<bool> shouldSort = false;

    {
        ImGui::Begin("Configure", &s_open);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / AppCtx::g_app->io->Framerate, AppCtx::g_app->io->Framerate);
        
        if (ImGui::BeginCombo("##combo1", AppCtx::g_app->categories[AppCtx::g_app->currentCategory])) // The second parameter is the label previewed before opening the combo.
        {
            for (size_t n = 0; n < AppCtx::g_app->categories.size(); ++n)
            {
                bool isSelected = (AppCtx::g_app->categories[AppCtx::g_app->currentCategory] == AppCtx::g_app->categories[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(AppCtx::g_app->categories[n], isSelected))
                    AppCtx::g_app->currentCategory = static_cast<SortCategory>(n);
                if (isSelected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        if(AppCtx::g_app->current_item >= AppCtx::g_app->items[AppCtx::g_app->currentCategory].size()) AppCtx::g_app->current_item = 0;

        if (ImGui::BeginCombo("##combo2", AppCtx::g_app->items[AppCtx::g_app->currentCategory][AppCtx::g_app->current_item])) // The second parameter is the label previewed before opening the combo.
        {
            for (size_t n = 0; n < AppCtx::g_app->items[AppCtx::g_app->currentCategory].size(); ++n)
            {
                bool isSelected = (AppCtx::g_app->items[AppCtx::g_app->currentCategory][AppCtx::g_app->current_item] == AppCtx::g_app->items[AppCtx::g_app->currentCategory][n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(AppCtx::g_app->items[AppCtx::g_app->currentCategory][n], isSelected))
                    AppCtx::g_app->current_item = n;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
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
                    AppCtx::g_app->currentDisplayType = static_cast<DisplayType>(n);
                if (isSelected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Checkbox("Color", &AppCtx::g_app->isColored);

        ImGui::Spacing();
        ImGui::SeparatorText("Variables");
        ImGui::Spacing();

        ImGui::InputFloat("Set Speed", &Sort::speed, 0.001f);
        Sort::speed = std::clamp(Sort::speed, 0.001f, 1000.f);

        ImGui::InputInt("Set Array Length", &Sort::length, 2);
        Sort::length = std::clamp(Sort::length, 2, 1024*10);

        if (AppCtx::g_app->currentCategory == 1 && AppCtx::g_app->current_item == 0)
            ImGui::SliderInt("Set Buckets/Radix", &AppCtx::g_app->setRadix, 2, 10, "%d");

        ImGui::Spacing();
        if (!(AppCtx::g_app->sorter->isSorting) && !(AppCtx::g_app->sorter->isShuffling))
        {
            if(ImGui::Button("Sort")) {
                LOGINFO("Starting sort");
                switch(AppCtx::g_app->currentCategory)
                {
                    case SortCategory::Exchange: {
                        switch(AppCtx::g_app->current_item)
                        {
                            SORTCASE(0, BubbleSort);
                            SORTCASE(1, QuickSort);
                            SORTCASE(2, CombSort);
                        }
                    } break;
                    case SortCategory::Distribution: {
                        switch(AppCtx::g_app->current_item)
                        {
                            SORTCASERADIX(0, RadixLSDSort);
                            SORTCASE(1, PigeonHoleSort);
                            SORTCASE(2, GravitySort);
                            SORTCASE(3, BogoSort);
                        }
                    } break;
                    
                    case SortCategory::Insertion: {
                        switch(AppCtx::g_app->current_item) {
                            SORTCASE(0, InsertionSort);
                        }
                    } break;
                    case SortCategory::Merge: {
                        switch(AppCtx::g_app->current_item) {
                            SORTCASE(0, MergeSort);
                        }
                    } break;
                    case SortCategory::Select: {
                        switch(AppCtx::g_app->current_item) {
                            SORTCASE(0, SelectionSort);
                        }
                    } break;   
                    default: LOGERR("Unknown sort category");
                }
                shouldSort = true;
            }
        } else {
            if(ImGui::Button("Stop")) {
                LOGINFO("Stopping sort");    
                AppCtx::g_app->sorter->wantStop = true;
                if (AppCtx::g_app->sortThread.has_value()) {
                    if (AppCtx::g_app->sortThread->joinable())
                        AppCtx::g_app->sortThread->join();
                    AppCtx::g_app->sortThread.reset();
                }
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Reverse instead of Shuffling", &AppCtx::g_app->reverse);
        ImGui::End();
    }
    
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), AppCtx::g_app->renderer);
    if(shouldSort)
    {
        if (AppCtx::g_app->sortThread.has_value()) {
            if (AppCtx::g_app->sortThread->joinable())
                AppCtx::g_app->sortThread->join();
            AppCtx::g_app->sortThread.reset();
        }

        AppCtx::g_app->sortThread = std::make_optional<std::thread>([&]() {
            shouldSort = false;
            if(!AppCtx::g_app->reverse)
                AppCtx::g_app->sorter->shuffle();
            else
                AppCtx::g_app->sorter->reverse();

            if (AppCtx::g_app->sorter->wantStop) return;

            if(!(AppCtx::g_app->sorter->wantStop)) {
                LOGINFO("Sorting");
                AppCtx::g_app->sorter->sort();
            }

            if (AppCtx::g_app->sorter->wantStop) return;
        });
    }
    if (AppCtx::g_app->sorter->wantClose) return 2;

    if (AppCtx::g_app->sorter->wantStop) {
        return 1;
    }
    
    return 0;
}