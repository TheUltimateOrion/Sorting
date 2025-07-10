#include "App.h"
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

SDL_Color SortRenderer::HSVToRGB(unsigned char hue, unsigned char sat, unsigned char value) const noexcept
{
    SDL_Color rgb;
    unsigned char region, remainder, p, q, t;
    region = hue / 43;
    remainder = (hue - (region * 43)) * 6; 
    
    p = (value * (0xFF - sat)) >> 8;
    q = (value * (0xFF - ((sat * remainder) >> 8))) >> 8;
    t = (value * (0xFF - ((sat * (0xFF - remainder)) >> 8))) >> 8;
    
    switch (region)
    {
        case 0:
            rgb.r = value; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = value; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = value; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = value;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = value;
            break;
        default:
            rgb.r = value; rgb.g = p; rgb.b = q;
            break;
    }
    
    return rgb;
}

void SortRenderer::renderText(const std::string& txt, float x, float y, SDL_Color color) const noexcept
{
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(app->font, txt.c_str(), 0, color); // SDL_Surface* textSurface = TTF_RenderText_Solid(app->font, txt.c_str(), color);
    SDL_Texture* text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
    float text_width = static_cast<float>(textSurface->w);
    float text_height = static_cast<float>(textSurface->h);
    SDL_DestroySurface(textSurface); // SDL_FreeSurface(textSurface);
    SDL_FRect renderQuad = { x, y, text_width, text_height};
    SDL_RenderTexture(app->renderer, text, nullptr, &renderQuad); // SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}

void SortRenderer::renderInfo() const noexcept
{
    Uint8 _r, _g, _b, _a; SDL_GetRenderDrawColor(app->renderer, &_r, &_g, &_b, &_a);
    SDL_FRect rect = {0.0f, 0.0f, 230.0f, 125.0f};
    SDL_SetRenderDrawColor(app->renderer, 0x40, 0x40, 0x40, 0x80);
    SDL_RenderFillRect(app->renderer, &rect);

    SDL_Color textColor = { 0, 0xFF, 0, 0 };
    if (app->sorter->isSorting)
        app->sorter->last_time = static_cast<float>(clock()) / 1000.0f - app->sorter->start_time;
    if (app->sorter->isSorting || (app->sorter->last_time == 0.0f))
        textColor = { 0xFF, 0xFF, 0xFF, 0 };
    
    renderText("TIME: " + std::to_string(app->sorter->last_time) + 's', 10.0f, 10.0f, textColor);
    renderText(std::string("Sort: ") + app->items[app->current_category][app->current_item], 10, 30, { 0xFF, 0xFF, 0xFF, 0 });

    renderText("Swaps: " + std::to_string(app->sorter->swaps), 10.0f, 50.0f, { 0xFF, 0xFF, 0xFF, 0 });
    renderText("Comparisions: " + std::to_string(app->sorter->comparisions), 10.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0 });

    if (app->sorter->isSorting)
        renderText("Sorting...", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    if (app->sorter->isShuffling)
        renderText("Shuffling...", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    if (!(app->sorter->isShuffling) && !(app->sorter->isSorting) && app->sorter->sorted)
        renderText("Sorted", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    SDL_SetRenderDrawColor(app->renderer, _r, _g, _b, _a);
}

void SortRenderer::update() noexcept
{
    {
        LOCK_GUARD;
        elems = app->data;
    }
    SDL_SetRenderDrawColor(app->renderer, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(app->renderer);

    app->current_element = app->sorter->first.load();

    SDL_Color sortColor; 
    for (int k = 0; k < elems.size(); k++)
    {
        sortColor = HSVToRGB(elems[k] * 0xFF / elems.size(), 0xFF, 0xFF);
        if (k == app->sorter->first || k == app->sorter->second)
            SDL_SetRenderDrawColor(app->renderer, 0xFF, 0x0, 0x0, 0xFF);
        else {
            SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            if (app->isColored) {
                //SDL_SetRenderDrawColor(renderer, elems[k] * 255 / (elems.size()), 0, 255-(elems[k] * 255 / (elems.size())), 255);
                SDL_SetRenderDrawColor(app->renderer, sortColor.r, sortColor.g, sortColor.b, 0xFF);
            }
        }
        const float spacing = WIN_WIDTH / elems.size();
        float degreesQuotient = 360.0f / static_cast<float>(elems.size());
        float radiansQuotient = (M_PI / 180);
        float size;
        SDL_Vertex vertices[3];
        
        switch (app->displayType) {
            case DisplayType::Bar: {
                // SDL_RenderDrawLine(renderer, k + 1, elems.size() , k + 1, elems.size()  - elems[k]);
                // std::cout << -elems[k] * spacing << std::endl;
                app->rect = (SDL_FRect) {k * spacing, WIN_HEIGHT, spacing, -(elems[k] * spacing * WIN_HEIGHT / WIN_WIDTH)};
                SDL_RenderFillRect(app->renderer, &app->rect);
            } break;
            case DisplayType::Dot: {
                // SDL_RenderDrawPoint(renderer, k + 1, elems.size()  - elems[k]);
                SDL_RenderPoint(app->renderer, (k + 1) * spacing, (WIN_HEIGHT - elems[k] * spacing * WIN_HEIGHT / WIN_WIDTH)); // SDL_RenderDrawPoint(app->renderer, (k + 1) * spacing, (elems.size() - elems[k]) * spacing);
            } break;
            case DisplayType::RainbowRectangle: {
                app->isColored = true;
                app->rect = (SDL_FRect){k * spacing, 0, spacing, WIN_HEIGHT};
                SDL_RenderFillRect(app->renderer, &app->rect);
            } break;
            case DisplayType::Circle: {
                app->isColored = true;
                float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
                SDL_GetRenderDrawColorFloat(app->renderer, &r, &g, &b, &a);
                size = std::min(WIN_HEIGHT, WIN_WIDTH) * 0.75 * 0.5;
                vertices[0] = {
                    {WIN_WIDTH / 2, WIN_HEIGHT / 2}, /* first point location */ 
                    { r, g, b, 0xFF }, /* first color */ 
                    { 0.f, 0.f }
                };
                vertices[1] = {
                    {WIN_WIDTH / 2 + size * cosf(radiansQuotient * k * degreesQuotient), WIN_HEIGHT / 2 + size * sinf(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                    { r, g, b, 0xFF }, /* second color */
                    { 0.f, 0.f }
                };
                vertices[2] = {
                    {WIN_WIDTH / 2 + size * cosf(radiansQuotient * (k + 1) * degreesQuotient), WIN_HEIGHT / 2 + size * sinf(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                    { r, g, b, 0xFF }, /* third color */
                    { 0.f, 0.f }
                };
                if (!SDL_RenderGeometry(app->renderer, nullptr, vertices, 3, nullptr, 0))
                {
                    LOGERR(SDL_GetError());
                }
            } break;
            case DisplayType::CircleDot: {
                //std::cout << elems[k] / (k + 1) << std::endl;
                // size = 200.0f * ((float)elems[k] / (float)(k + 1));
                size = std::min(WIN_HEIGHT, WIN_WIDTH) * 0.75 * 0.5;
                SDL_RenderPoint(app->renderer, WIN_WIDTH / 2 + size * cosf(radiansQuotient * k * degreesQuotient), WIN_HEIGHT / 2 + size * sinf(radiansQuotient * k * degreesQuotient));
            } break;
            case DisplayType::DisparityCircle: {
                size = std::min(WIN_HEIGHT, WIN_WIDTH) * 0.75 * 0.5;
                SDL_RenderLine(app->renderer, WIN_WIDTH / 2 + size * cosf(radiansQuotient * elems[k] * degreesQuotient), WIN_HEIGHT / 2 + size * sinf(radiansQuotient * elems[k] * degreesQuotient), WIN_WIDTH / 2 + size * cosf(radiansQuotient * (k + 1) * degreesQuotient), WIN_HEIGHT / 2 + size * sinf(radiansQuotient * (k + 1) * degreesQuotient));
            } break;
            case DisplayType::Spiral: {
                float r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColorFloat(app->renderer, &r, &g, &b, &a);
                size = 0.5f * std::min(WIN_HEIGHT, WIN_WIDTH) / static_cast<float>(elems.size());
                vertices[0] = {
                    {WIN_WIDTH / 2, WIN_HEIGHT / 2}, /* first point location */ 
                    { r, g, b, 0xFF }, /* first color */ 
                    { 0.f, 0.f }
                };
                vertices[1] = {
                    {WIN_WIDTH / 2 + size * elems[k] * cosf(radiansQuotient * k * degreesQuotient), WIN_HEIGHT / 2 + size * elems[k] * sinf(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                    { r, g, b, 0xFF }, /* second color */
                    { 0.f, 0.f }
                };
                vertices[2] = {
                    {WIN_WIDTH / 2 + size * elems[k] * cosf(radiansQuotient * (k + 1) * degreesQuotient), WIN_HEIGHT / 2 + size * elems[k] * sinf(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                    { r, g, b, 0xFF }, /* third color */
                    { 0.f, 0.f }
                };
                // SDL_RenderDrawPoint(renderer, WIN_WIDTH / 2 + 50 * (k / elems[k]) * cos(radiansQuotient * (k + 1) * degreesQuotient), WIN_WIDTH / 2 + 50 * (k / elems[k]) * sin(radiansQuotient * (k + 1) * degreesQuotient));
                SDL_RenderGeometry(app->renderer, nullptr, vertices, 3, nullptr, 0);
            } break;
            case DisplayType::SpiralDot: {
                size = 0.5f * std::min(WIN_HEIGHT, WIN_WIDTH) / static_cast<float>(elems.size());
                SDL_RenderPoint(app->renderer, WIN_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), WIN_HEIGHT / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient)); // SDL_RenderDrawPoint(app->renderer, WIN_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient));
            } break;
        }
    }

    renderInfo();

    int ret = renderGUI();
    if (ret == 1) {
        app->sorter->isShuffling = false;
        app->sorter->isSorting = false;
        app->sorter->sorted = true;
        app->sorter->wantStop = false;
        return;
    } else if (ret == 2) return;
        
    SDL_RenderPresent(app->renderer);
    if(SDL_PollEvent(&app->event))
    {
        ImGui_ImplSDL3_ProcessEvent(&app->event);
        if (app->event.type == SDL_EVENT_QUIT)
        {
            app->sorter->wantClose = true;
            return;
        }
    }

    std::this_thread::sleep_for(1ms);
    
}

static bool p_open = true;

int SortRenderer::renderGUI() const noexcept
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if(!p_open) {
        app->sorter->wantClose = true;
        
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), app->renderer);
        return 2;
    }

    std::atomic<bool> shouldSort = false;

    {
        ImGui::Begin("Configure", &p_open);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / app->io->Framerate, app->io->Framerate);
        
        if (ImGui::BeginCombo("##combo1", app->categories[app->current_category])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < app->categories.size(); n++)
            {
                bool is_selected = (app->categories[app->current_category] == app->categories[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(app->categories[n], is_selected))
                    app->current_category = static_cast<SortCategory>(n);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        if(app->current_item >= app->items[app->current_category].size()) app->current_item = 0;

        if (ImGui::BeginCombo("##combo2", app->items[app->current_category][app->current_item])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < app->items[app->current_category].size(); n++)
            {
                bool is_selected = (app->items[app->current_category][app->current_item] == app->items[app->current_category][n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(app->items[app->current_category][n], is_selected))
                    app->current_item = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Display Config");
        ImGui::Spacing();

        if (ImGui::BeginCombo("##combo3", app->displayTypes[app->displayType])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < app->displayTypes.size(); n++)
            {
                bool is_selected = (app->displayTypes[app->displayType] == app->displayTypes[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(app->displayTypes[n], is_selected))
                    app->displayType = static_cast<DisplayType>(n);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Checkbox("Color", &app->isColored);

        ImGui::Spacing();
        ImGui::SeparatorText("Variables");
        ImGui::Spacing();

        ImGui::InputFloat("Set Speed", &Sort::speed, 0.001f);
        Sort::speed = std::clamp(Sort::speed, 0.001f, 1000.f);

        ImGui::InputInt("Set Array Length", &Sort::length, 2);
        Sort::length = std::clamp(Sort::length, 2, 1024*10);

        if (app->current_category == 1 && app->current_item == 0)
            ImGui::SliderInt("Set Buckets/Radix", &app->setRadix, 2, 10, "%d");

        ImGui::Spacing();
        if (!(app->sorter->isSorting) && !(app->sorter->isShuffling))
        {
            if(ImGui::Button("Sort")) {
                LOGINFO("Starting sort");
                switch(app->current_category)
                {
                    case SortCategory::Exchange: {
                        switch(app->current_item)
                        {
                            SORTCASE(0, BubbleSort);
                            SORTCASE(1, QuickSort);
                            SORTCASE(2, CombSort);
                        }
                    } break;
                    case SortCategory::Distribution: {
                        switch(app->current_item)
                        {
                            SORTCASERADIX(0, RadixLSDSort);
                            SORTCASE(1, PigeonHoleSort);
                            SORTCASE(2, GravitySort);
                            SORTCASE(3, BogoSort);
                        }
                    } break;
                    
                    case SortCategory::Insertion: {
                        switch(app->current_item) {
                            SORTCASE(0, InsertionSort);
                        }
                    } break;
                    case SortCategory::Merge: {
                        switch(app->current_item) {
                            SORTCASE(0, MergeSort);
                        }
                    } break;
                    case SortCategory::Select: {
                        switch(app->current_item) {
                            SORTCASE(0, SelectionSort);
                        }
                    } break;   
                    default:
                        std::cout << "Invalid Sort!" << std::endl;
                }
                shouldSort = true;
            }
        } else {
            if(ImGui::Button("Stop")) {
                LOGINFO("Stopping sort");    
                app->sorter->wantStop = true;
                if (app->sortThread.has_value()) {
                    if (app->sortThread->joinable())
                        app->sortThread->join();
                    app->sortThread.reset();
                }
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Reverse instead of Shuffling", &app->reverse);
        ImGui::End();
    }
    
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), app->renderer);
    if(shouldSort)
    {
        if (app->sortThread.has_value()) {
            if (app->sortThread->joinable())
                app->sortThread->join();
            app->sortThread.reset();
        }

        app->sortThread = std::make_optional<std::thread>([&]() {
            shouldSort = false;
            if(!app->reverse)
                app->sorter->shuffle();
            else
                app->sorter->reverse();

            if (app->sorter->wantStop) return;

            if(!(app->sorter->wantStop)) {
                LOGINFO("Sorting");
                app->sorter->sort();
            }

            if (app->sorter->wantStop) return;
        });
    }
    if (app->sorter->wantClose) return 2;

    if (app->sorter->wantStop) {
        return 1;
    }
    
    return 0;
}