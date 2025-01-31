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

SDL_Color SortRenderer::HSVToRGB(unsigned char hue, unsigned char sat, unsigned char value)
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

void SortRenderer::renderText(std::string txt, int x, int y, SDL_Color color)
{
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(app->font, txt.c_str(), 0, color); // SDL_Surface* textSurface = TTF_RenderText_Solid(app->font, txt.c_str(), color);
    SDL_Texture* text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_DestroySurface(textSurface); // SDL_FreeSurface(textSurface);
    SDL_FRect renderQuad = { x, y, text_width, text_height };
    SDL_RenderTexture(app->renderer, text, NULL, &renderQuad); // SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}

void SortRenderer::renderInfo()
{
    Uint8 _r, _g, _b, _a; SDL_GetRenderDrawColor(app->renderer, &_r, &_g, &_b, &_a);
    SDL_FRect rect = {0.0f, 0.0f, 230.0f, 125.0f};
    SDL_SetRenderDrawColor(app->renderer, 0x40, 0x40, 0x40, 0x80);
    SDL_RenderFillRect(app->renderer, &rect);

    SDL_Color textColor = { 0, 0xFF, 0, 0 };
    if (app->sorter->isSorting)
        ::last_time = (float)clock() / 1000.0f - app->sorter->start_time;
    if (app->sorter->isSorting || (::last_time == 0.0f))
        textColor = { 0xFF, 0xFF, 0xFF, 0 };
    
    renderText("TIME: " + std::to_string(::last_time) + 's', 10, 10, textColor);
    renderText(std::string("Sort: ") + app->items[app->current_category][app->current_item], 10, 30, { 0xFF, 0xFF, 0xFF, 0 });

    app->swaps = app->sorter->swaps;
    app->comparisions = app->sorter->comparisions;
    renderText("Swaps: " + std::to_string(app->swaps), 10, 50, { 0xFF, 0xFF, 0xFF, 0 });
    renderText("Comparisions: " + std::to_string(app->comparisions), 10, 70, { 0xFF, 0xFF, 0xFF, 0 });

    if (app->sorter->isSorting)
        renderText("Sorting...", 10, 90, { 0xFF, 0xFF, 0xFF, 0 });
    if (app->sorter->isShuffling)
        renderText("Shuffling...", 10, 90, { 0xFF, 0xFF, 0xFF, 0 });
    if (!(app->sorter->isShuffling) && !(app->sorter->isSorting) && app->sorter->sorted)
        renderText("Sorted", 10, 90, { 0xFF, 0xFF, 0xFF, 0 });
    SDL_SetRenderDrawColor(app->renderer, _r, _g, _b, _a);
}

void SortRenderer::update(std::vector<int>& elems, int a, int b)
{
    app->calculateDeltaTime();
    SDL_SetRenderDrawColor(app->renderer, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(app->renderer);

    app->current_element = a;

    SDL_Color sortColor; 
    for (int k = 0; k < elems.size(); k++)
    {
        sortColor = HSVToRGB(elems[k] * 0xFF / elems.size(), 0xFF, 0xFF);
        if (k == a || k == b)
            SDL_SetRenderDrawColor(app->renderer, 0xFF, 0x0, 0x0, 0xFF);
        else {
            SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            if (app->isColored) {
                //SDL_SetRenderDrawColor(renderer, elems[k] * 255 / (elems.size()), 0, 255-(elems[k] * 255 / (elems.size())), 255);
                SDL_SetRenderDrawColor(app->renderer, sortColor.r, sortColor.g, sortColor.b, 0xFF);
            }
        }
        float spacing = WIN_WIDTH / elems.size();
        float degreesQuotient = 360.0f / (float)elems.size();
        float radiansQuotient = (M_PI / 180);
        float size;
        SDL_Vertex vertices[3];
        
        switch (app->displayType) {
            case 0: {
                // SDL_RenderDrawLine(renderer, k + 1, elems.size() , k + 1, elems.size()  - elems[k]);
                // std::cout << -elems[k] * spacing << std::endl;
                app->rect = (SDL_FRect) {k * spacing, WIN_HEIGHT, spacing, -(elems[k] * spacing * WIN_HEIGHT / WIN_WIDTH)};
                SDL_RenderFillRect(app->renderer, &app->rect);
            } break;
            case 1: {
                // SDL_RenderDrawPoint(renderer, k + 1, elems.size()  - elems[k]);
                SDL_RenderPoint(app->renderer, (k + 1) * spacing, (WIN_HEIGHT - elems[k] * spacing * WIN_HEIGHT / WIN_WIDTH)); // SDL_RenderDrawPoint(app->renderer, (k + 1) * spacing, (elems.size() - elems[k]) * spacing);
            } break;
            case 2: {
                app->isColored = true;
                app->rect = (SDL_FRect){k * spacing, WIN_WIDTH, spacing, -(WIN_WIDTH / 2)};
                SDL_RenderFillRect(app->renderer, &app->rect);
            } break;
            case 3: {
                app->isColored = true;
                float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
                SDL_GetRenderDrawColor(app->renderer, reinterpret_cast<Uint8*>(&r), reinterpret_cast<Uint8*>(&g), reinterpret_cast<Uint8*>(&b), reinterpret_cast<Uint8*>(&a));
                size = 200.0f;
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
                SDL_RenderGeometry(app->renderer, NULL, vertices, 3, NULL, 0);
            } break;
            case 4: {
                //std::cout << elems[k] / (k + 1) << std::endl;
                size = 200.0f * ((float)elems[k] / (float)(k + 1));
                SDL_RenderPoint(app->renderer, WIN_WIDTH / 2 + size * cos(radiansQuotient * k * degreesQuotient), WIN_WIDTH / 2 + size * sin(radiansQuotient * k * degreesQuotient));
            } break;
            case 5: {
                size = 200.0f;
                SDL_RenderLine(app->renderer, WIN_WIDTH / 2 + size * cos(radiansQuotient * elems[k] * degreesQuotient), WIN_WIDTH / 2 + size * sin(radiansQuotient * elems[k] * degreesQuotient), WIN_WIDTH / 2 + size * cos(radiansQuotient * (k + 1) * degreesQuotient), WIN_WIDTH / 2 + size * sin(radiansQuotient * (k + 1) * degreesQuotient));
            } break;
            case 6: {
                float r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColor(app->renderer, reinterpret_cast<Uint8*>(&r), reinterpret_cast<Uint8*>(&g), reinterpret_cast<Uint8*>(&b), reinterpret_cast<Uint8*>(&a));
                size = (0.5f * (float)WIN_WIDTH / (float)elems.size());
                if (app->isColored) {
                    vertices[0] = {
                        {WIN_WIDTH / 2, WIN_WIDTH / 2}, /* first point location */ 
                        { r, g, b, 0xFF }, /* first color */ 
                        { 0.f, 0.f }
                    };
                    vertices[1] = {
                        {WIN_WIDTH / 2 + size * elems[k] * cosf(radiansQuotient * k * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sinf(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                        { r, g, b, 0xFF }, /* second color */
                        { 0.f, 0.f }
                    };
                    vertices[2] = {
                        {WIN_WIDTH / 2 + size * elems[k] * cosf(radiansQuotient * (k + 1) * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sinf(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                        { r, g, b, 0xFF }, /* third color */
                        { 0.f, 0.f }
                    };
                } else {
                    vertices[0] = {
                        {WIN_WIDTH / 2, WIN_WIDTH / 2}, /* first point location */ 
                        { r, g, b, 0xFF }, /* first color */ 
                        { 0.f, 0.f }
                    };
                    vertices[1] = {
                        {WIN_WIDTH / 2 + size * elems[k] * cosf(radiansQuotient * k * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sinf(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                        { r, g, b, 0xFF }, /* second color */
                        { 0.f, 0.f }
                    };
                    vertices[2] = {
                        {WIN_WIDTH / 2 + size  * elems[k] * cosf(radiansQuotient * (k + 1) * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sinf(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                        { r, g, b, 0xFF }, /* third color */
                        { 0.f, 0.f }
                    };
                }
                // SDL_RenderDrawPoint(renderer, WIN_WIDTH / 2 + 50 * (k / elems[k]) * cos(radiansQuotient * (k + 1) * degreesQuotient), WIN_WIDTH / 2 + 50 * (k / elems[k]) * sin(radiansQuotient * (k + 1) * degreesQuotient));
                SDL_RenderGeometry(app->renderer, NULL, vertices, 3, NULL, 0);
            } break;
            case 7: {
                size = (0.5f * (float)WIN_WIDTH / (float)elems.size());
                SDL_RenderPoint(app->renderer, WIN_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient)); // SDL_RenderDrawPoint(app->renderer, WIN_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), WIN_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient));
            } break;
        }
    }

    renderInfo();

    int ret = renderGUI();
    if (ret == 1) {
        app->sorter->isShuffling = false;
        app->sorter->isSorting = false;
        app->sorter->sorted = true;
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
    // SDL_Delay(1 / (app->sorter->speed));
}

static bool p_open = true;
int SortRenderer::renderGUI()
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
    bool shouldSort = false;
    {
        ImGui::Begin("Configure", &p_open);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / app->sorter->io->Framerate, app->sorter->io->Framerate);
        
        if (ImGui::BeginCombo("##combo1", app->categories[app->current_category])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < app->categories.capacity(); n++)
            {
                bool is_selected = (app->categories[app->current_category] == app->categories[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(app->categories[n], is_selected))
                    app->current_category = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        if(app->current_item >= app->items[app->current_category].capacity()) app->current_item = 0;

        if (ImGui::BeginCombo("##combo2", app->items[app->current_category][app->current_item])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < app->items[app->current_category].capacity(); n++)
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
            for (int n = 0; n < app->displayTypes.capacity(); n++)
            {
                bool is_selected = (app->displayTypes[app->displayType] == app->displayTypes[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(app->displayTypes[n], is_selected))
                    app->displayType = n;
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

        app->setSpeed = std::clamp((double)app->setSpeed, 0.001, 1000.0);
        ImGui::InputFloat("Set Speed", &app->setSpeed, 0.001f);

        app->setLength = std::clamp(app->setLength, 2, 1024*10);
        ImGui::InputInt("Set Length", &app->setLength, 2);

        if (app->current_category == 1 && app->current_item == 0)
            ImGui::SliderInt("Set Buckets/Radix", &app->setRadix, 2, 10, "%d");

        ImGui::Spacing();
        if (!(app->sorter->isSorting) && !(app->sorter->isShuffling))
        {
            if(ImGui::Button("Sort")) {
                LOGINFO("Starting sort");
                switch(app->current_category)
                {
                    case 0: {
                        switch(app->current_item)
                        {
                            SORTCASE(0, BubbleSort);
                            SORTCASE(1, QuickSort);
                            SORTCASE(2, CombSort);
                        }
                    } break;
                    case 1: {
                        switch(app->current_item)
                        {
                            SORTCASERADIX(0, RadixLSDSort);
                            SORTCASE(1, PigeonHoleSort);
                            SORTCASE(2, GravitySort);
                            SORTCASE(3, BogoSort);
                        }
                    } break;
                    
                    case 2: {
                        switch(app->current_item) {
                            SORTCASE(0, InsertionSort);
                        }
                    } break;
                    case 3: {
                        switch(app->current_item) {
                            SORTCASE(0, MergeSort);
                        }
                    } break;
                    case 4: {
                        switch(app->current_item) {
                            SORTCASE(0, SelectionSort);
                        }
                        
                        _jmp:
                        app->sorter->setSpeed(app->setSpeed);
                        app->sorter->setLength(app->setLength);
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
        shouldSort = false;
        if(!app->reverse)
            app->sorter->shuffle();
        else
            app->sorter->reverse();
        if (app->sorter->wantClose) return 2;
        if(!(app->sorter->wantStop)) {
            LOGINFO("Sorting");
            app->sorter->sort();
        }
        if (app->sorter->wantStop) {
            return 1;
        }
        if (app->sorter->wantClose) return 2;
    }
    return 0;
}