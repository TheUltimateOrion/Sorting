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
    
    p = (value * (255 - sat)) >> 8;
    q = (value * (255 - ((sat * remainder) >> 8))) >> 8;
    t = (value * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;
    
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
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(app->font, txt.c_str(), color);
    SDL_Texture* text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { x, y, text_width, text_height };
    SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}

void SortRenderer::renderInfo(Sort*& sort)
{
    SDL_Color textColor = { 0, 255, 0, 0 };
    if (sort->isSorting)
        ::last_time = (float)clock() / 1000.0f - sort->start_time;
    if (sort->isSorting || (::last_time == 0.0f))
        textColor = { 255, 255, 255, 0 };
    
    renderText("TIME: " + std::to_string(::last_time) + 's', 10, 10, textColor);
    renderText(std::string("Sort: ") + app->items[app->current_category][app->current_item], 10, 30, { 255, 255, 255, 0 });

    app->swaps = sort->swaps;
    app->comparisions = sort->comparisions;
    renderText("Swaps: " + std::to_string(app->swaps), 10, 50, { 255, 255, 255, 0 });
    renderText("Comparisions: " + std::to_string(app->comparisions), 10, 70, { 255, 255, 255, 0 });

    if (sort->isSorting)
        renderText("Sorting...", 10, 90, { 255, 255, 255, 0 });
    if (sort->isShuffling)
        renderText("Shuffling...", 10, 90, { 255, 255, 255, 0 });
    if (!(sort->isShuffling) && !(sort->isSorting) && sort->sorted)
        renderText("Sorted", 10, 90, { 255, 255, 255, 0 });
}

void SortRenderer::render(Sort* sort, std::vector<int>& elems, int a, int b)
{
    app->calculateDeltaTime();
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
    SDL_RenderClear(app->renderer);

    SDL_Color sortColor; 
    for (int k = 0; k < elems.size(); k++)
    {
        sortColor = HSVToRGB(elems[k] * 255 / elems.size(), 255, 255);
        if (k == a || k == b)
            SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
        else {
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
            if (app->isColored) {
                //SDL_SetRenderDrawColor(renderer, elems[k] * 255 / (elems.size()), 0, 255-(elems[k] * 255 / (elems.size())), 255);
                SDL_SetRenderDrawColor(app->renderer, sortColor.r, sortColor.g, sortColor.b, 255);
            }
        }
        int spacing = LOGICAL_WIDTH / (int)elems.size();
        float degreesQuotient = 360.0f / (float)elems.size();
        float radiansQuotient = (M_PI / 180);
        float size;
        SDL_Vertex vertices[3];
        
        switch (app->displayType) {
            case 0: {
                // SDL_RenderDrawLine(renderer, k + 1, elems.size() , k + 1, elems.size()  - elems[k]);
                app->rect = (SDL_Rect){k * spacing, LOGICAL_WIDTH, spacing, -elems[k] * spacing};
                SDL_RenderFillRect(app->renderer, &app->rect);
            } break;
            case 1: {
                // SDL_RenderDrawPoint(renderer, k + 1, elems.size()  - elems[k]);
                SDL_RenderDrawPoint(app->renderer, (k + 1) * spacing, (elems.size() - elems[k]) * spacing);
            } break;
            case 2: {
                app->isColored = true;
                app->rect = (SDL_Rect){k * spacing, LOGICAL_WIDTH, spacing, -(LOGICAL_WIDTH / 2)};
                SDL_RenderFillRect(app->renderer, &app->rect);
            } break;
            case 3: {
                app->isColored = true;
                Uint8 r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColor(app->renderer, &r, &g, &b, &a);
                size = 200.0f;
                vertices[0] = {
                    {LOGICAL_WIDTH / 2, LOGICAL_WIDTH / 2}, /* first point location */ 
                    { r, g, b, 0xFF }, /* first color */ 
                    { 0.f, 0.f }
                };
                vertices[1] = {
                    {LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                    { r, g, b, 0xFF }, /* second color */
                    { 0.f, 0.f }
                };
                vertices[2] = {
                    {LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * (k + 1) * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                    { r, g, b, 0xFF }, /* third color */
                    { 0.f, 0.f }
                };
                SDL_RenderGeometry(app->renderer, NULL, vertices, 3, NULL, 0);
            } break;
            case 4: {
                //std::cout << elems[k] / (k + 1) << std::endl;
                size = 200.0f * ((float)elems[k] / (float)(k + 1));
                SDL_RenderDrawPoint(app->renderer, LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * k * degreesQuotient));
            } break;
            case 5: {
                size = 200.0f;
                SDL_RenderDrawLine(app->renderer, LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * elems[k] * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * elems[k] * degreesQuotient), LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * (k + 1) * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * (k + 1) * degreesQuotient));
            } break;
            case 6: {
                Uint8 r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColor(app->renderer, &r, &g, &b, &a);
                size = (0.5f * (float)LOGICAL_WIDTH / (float)elems.size());
                if (app->isColored) {
                    vertices[0] = {
                        {LOGICAL_WIDTH / 2, LOGICAL_WIDTH / 2}, /* first point location */ 
                        { r, g, b, 0xFF }, /* first color */ 
                        { 0.f, 0.f }
                    };
                    vertices[1] = {
                        {LOGICAL_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                        { r, g, b, 0xFF }, /* second color */
                        { 0.f, 0.f }
                    };
                    vertices[2] = {
                        {LOGICAL_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * (k + 1) * degreesQuotient), LOGICAL_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                        { r, g, b, 0xFF }, /* third color */
                        { 0.f, 0.f }
                    };
                } else {
                    vertices[0] = {
                        {LOGICAL_WIDTH / 2, LOGICAL_WIDTH / 2}, /* first point location */ 
                        { r, g, b, 0xFF }, /* first color */ 
                        { 0.f, 0.f }
                    };
                    vertices[1] = {
                        {LOGICAL_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient)}, /* second point location */ 
                        { r, g, b, 0xFF }, /* second color */
                        { 0.f, 0.f }
                    };
                    vertices[2] = {
                        {LOGICAL_WIDTH / 2 + size  * elems[k] * cos(radiansQuotient * (k + 1) * degreesQuotient), LOGICAL_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * (k + 1) * degreesQuotient)}, /* third point location */ 
                        { r, g, b, 0xFF }, /* third color */
                        { 0.f, 0.f }
                    };
                }
                // SDL_RenderDrawPoint(renderer, LOGICAL_WIDTH / 2 + 50 * (k / elems[k]) * cos(radiansQuotient * (k + 1) * degreesQuotient), LOGICAL_WIDTH / 2 + 50 * (k / elems[k]) * sin(radiansQuotient * (k + 1) * degreesQuotient));
                SDL_RenderGeometry(app->renderer, NULL, vertices, 3, NULL, 0);
            } break;
            case 7: {
                size = (0.5f * (float)LOGICAL_WIDTH / (float)elems.size());
                SDL_RenderDrawPoint(app->renderer, LOGICAL_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient));
            } break;
        }
    }

    renderInfo(sort);

    int ret = renderGUI(sort);
    if (ret == 1)
        return;
        
    SDL_RenderPresent(app->renderer);
    if(SDL_PollEvent(&app->event))
    {
        ImGui_ImplSDL2_ProcessEvent(&app->event);
        if (app->event.type == SDL_QUIT)
        {
            sort->wantClose = true;
            return;
        }
    }
    SDL_Delay(1 / (sort->speed));
}

bool SortRenderer::renderGUI(Sort* sort)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    bool shouldSort = false;
    {
        ImGui::Begin("Configure");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / sort->io->Framerate, sort->io->Framerate);

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

        ImGui::SameLine();
        ImGui::Checkbox("Color", &app->isColored);

        ImGui::Spacing();
        ImGui::SeparatorText("Display Types");
        ImGui::Spacing();

        ImGui::RadioButton("Bar", &app->displayType, 0);                 ImGui::SameLine();
        ImGui::RadioButton("Dot", &app->displayType, 1);                 ImGui::SameLine();
        ImGui::RadioButton("Rainbow Rectangle", &app->displayType, 2);   ImGui::SameLine();
        ImGui::RadioButton("Circle", &app->displayType, 3);              
        ImGui::RadioButton("Circle Dot", &app->displayType, 4);          ImGui::SameLine();
        ImGui::RadioButton("Disparity Circle", &app->displayType, 5);    ImGui::SameLine();
        ImGui::RadioButton("Spiral", &app->displayType, 6);              ImGui::SameLine();
        ImGui::RadioButton("Spiral Dot", &app->displayType, 7);

        ImGui::Spacing();
        ImGui::SeparatorText("Variables");
        ImGui::Spacing();

        app->setSpeed = std::clamp((double)app->setSpeed, 0.001, 1000.0);
        ImGui::InputFloat("Set Speed", &app->setSpeed, 0.001f);

        app->setLength = std::clamp(app->setLength, 2, 512);
        ImGui::InputInt("Set Length", &app->setLength, 2);

        if (app->current_item == 6)
            ImGui::SliderInt("Set Buckets/Radix", &app->setRadix, 2, 10, "%d");

        ImGui::Spacing();
        if (!(sort->isSorting) && !(sort->isShuffling))
        {
            if(ImGui::Button("Sort")) {
                switch(app->current_category)
                {
                    case 0: {
                        switch(app->current_item)
                        {
                            case 0: {
                                sort = new BubbleSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                            case 1: {
                                sort = new QuickSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                            case 2: {
                                sort = new CombSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;

                        }
                    } break;
                    case 1: {
                        switch(app->current_item)
                        {
                            case 0: {
                                sort = new RadixLSDSort(sort->elems, sort->io, app->setRadix);
                                goto _jmp;
                            } break;
                            case 1: {
                                sort = new PigeonHoleSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                            case 2: {
                                sort = new GravitySort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                            case 3: {
                                sort = new BogoSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                        }
                    } break;
                    
                    case 2: {
                        switch(app->current_item) {
                            case 0: {
                                sort = new InsertionSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                        }
                    } break;
                    case 3: {
                        switch(app->current_item) {
                            case 0: {
                                sort = new MergeSort(sort->elems, sort->io);
                                goto _jmp;
                            } break;
                        }
                    } break;
                    case 4: {
                        switch(app->current_item) {
                            case 0: {
                                sort = new SelectionSort(sort->elems, sort->io);
                            } break;
                        }
                        
                        _jmp:
                        sort->setSpeed(app->setSpeed);
                        sort->setLength(app->setLength);
                    } break;   
                    default:
                        std::cout << "Invalid Sort!" << std::endl;
                }
                shouldSort = true;
            }
        } else {
            if(ImGui::Button("Stop"))
                sort->wantStop = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Reverse instead of Shuffling", &app->reverse);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    if(shouldSort)
    {
        shouldSort = false;
        if(!app->reverse)
            sort->shuffle();
        else
            sort->reverse();
        if (sort->wantClose) return 2;
        if(!(sort->wantStop))
            sort->sort();
        if (sort->wantStop) {
            return 1;
        }
        if (sort->wantClose) return 2;
    }
    return 0;
}