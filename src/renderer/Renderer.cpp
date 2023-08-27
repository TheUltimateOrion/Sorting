#include "main.h"
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

static float setSpeed = 1.0f;
static bool isColored = false;
static int displayType = 0;
static const char *items[] = { "BubbleSort", "SelectionSort", "InsertionSort", "QuickSort", "GravitySort", "PigeonHoleSort", "RadixLSDSort", "CombSort", "BogoSort"};
static int current_item = 0;
static bool isRadix = false;
static int setRadix = 2;
int setLength = 512;
static SDL_Rect rect;
unsigned int swaps = 0;
unsigned int comparisions = 0;
static bool reverse = false;

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
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, txt.c_str(), color);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { x, y, text_width, text_height };
    SDL_RenderCopy(renderer, text, NULL, &renderQuad);
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
     renderText(std::string("Sort: ") + items[current_item], 10, 30, { 255, 255, 255, 0 });

    swaps = sort->swaps;
    comparisions = sort->comparisions;
    renderText("Swaps: " + std::to_string(swaps), 10, 50, { 255, 255, 255, 0 });
    renderText("Comparisions: " + std::to_string(comparisions), 10, 70, { 255, 255, 255, 0 });

    if (sort->isSorting)
        renderText("Sorting...", 10, 90, { 255, 255, 255, 0 });
    if (sort->isShuffling)
        renderText("Shuffling...", 10, 90, { 255, 255, 255, 0 });
    if (!(sort->isShuffling) && !(sort->isSorting) && sort->sorted)
        renderText("Sorted", 10, 90, { 255, 255, 255, 0 });
}

void SortRenderer::render(Sort* sort, std::vector<int>& elems, int a, int b)
{
    ::calculateDeltaTime();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Color sortColor; 
    for (int k = 0; k < elems.size(); k++)
    {
        sortColor = HSVToRGB(elems[k] * 255 / elems.size(), 255, 255);
        if (k == a || k == b)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            if (isColored) {
                //SDL_SetRenderDrawColor(renderer, elems[k] * 255 / (elems.size()), 0, 255-(elems[k] * 255 / (elems.size())), 255);
                SDL_SetRenderDrawColor(renderer, sortColor.r, sortColor.g, sortColor.b, 255);
            }
        }
        int spacing = LOGICAL_WIDTH / (int)elems.size();
        float degreesQuotient = 360.0f / (float)elems.size();
        float radiansQuotient = (M_PI / 180);
        float size;
        SDL_Vertex vertices[3];
        
        switch (displayType) {
            case 0: {
                // SDL_RenderDrawLine(renderer, k + 1, elems.size() , k + 1, elems.size()  - elems[k]);
                rect = (SDL_Rect){k * spacing, LOGICAL_WIDTH, spacing, -elems[k] * spacing};
                SDL_RenderFillRect(renderer, &rect);
            } break;
            case 1: {
                // SDL_RenderDrawPoint(renderer, k + 1, elems.size()  - elems[k]);
                SDL_RenderDrawPoint(renderer, (k + 1) * spacing, (elems.size() - elems[k]) * spacing);
            } break;
            case 2: {
                isColored = true;
                rect = (SDL_Rect){k * spacing, LOGICAL_WIDTH, spacing, -(LOGICAL_WIDTH / 2)};
                SDL_RenderFillRect(renderer, &rect);
            } break;
            case 3: {
                isColored = true;
                Uint8 r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
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
                SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
            } break;
            case 4: {
                //std::cout << elems[k] / (k + 1) << std::endl;
                size = 200.0f * ((float)elems[k] / (float)(k + 1));
                SDL_RenderDrawPoint(renderer, LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * k * degreesQuotient));
            } break;
            case 5: {
                size = 200.0f;
                SDL_RenderDrawLine(renderer, LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * elems[k] * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * elems[k] * degreesQuotient), LOGICAL_WIDTH / 2 + size * cos(radiansQuotient * (k + 1) * degreesQuotient), LOGICAL_WIDTH / 2 + size * sin(radiansQuotient * (k + 1) * degreesQuotient));
            } break;
            case 6: {
                Uint8 r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
                size = (0.5f * (float)LOGICAL_WIDTH / (float)elems.size());
                if (isColored) {
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
                SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
            } break;
            case 7: {
                size = (0.5f * (float)LOGICAL_WIDTH / (float)elems.size());
                SDL_RenderDrawPoint(renderer, LOGICAL_WIDTH / 2 + size * elems[k] * cos(radiansQuotient * k * degreesQuotient), LOGICAL_WIDTH / 2 + size * elems[k] * sin(radiansQuotient * k * degreesQuotient));
            } break;
        }
    }

    renderInfo(sort);

    int ret = renderGUI(sort);
    if (ret == 1)
        return;
        
    SDL_RenderPresent(renderer);
    if(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
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
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / sort->io.Framerate, sort->io.Framerate);

        if (ImGui::BeginCombo("##combo", items[current_item])) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (items[current_item] == items[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(items[n], is_selected))
                    current_item = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        } 

        ImGui::SameLine();
        ImGui::Checkbox("Color", &isColored);

        ImGui::Spacing();
        ImGui::SeparatorText("Display Types");
        ImGui::Spacing();

        ImGui::RadioButton("Bar", &displayType, 0);                 ImGui::SameLine();
        ImGui::RadioButton("Dot", &displayType, 1);                 ImGui::SameLine();
        ImGui::RadioButton("Rainbow Rectangle", &displayType, 2);   ImGui::SameLine();
        ImGui::RadioButton("Circle", &displayType, 3);              
        ImGui::RadioButton("Circle Dot", &displayType, 4);          ImGui::SameLine();
        ImGui::RadioButton("Circle Line", &displayType, 5);         ImGui::SameLine();
        ImGui::RadioButton("Spiral", &displayType, 6);              ImGui::SameLine();
        ImGui::RadioButton("Spiral Dot", &displayType, 7);

        ImGui::Spacing();
        ImGui::SeparatorText("Variables");
        ImGui::Spacing();

        setSpeed = std::clamp((double)setSpeed, 0.001, 1000.0);
        ImGui::InputFloat("Set Speed", &setSpeed, 0.001f);

        setLength = std::clamp(setLength, 2, 512);
        ImGui::InputInt("Set Length", &setLength, 2);

        if (current_item == 6)
            ImGui::SliderInt("Set Buckets/Radix", &setRadix, 2, 10, "%d");

        ImGui::Spacing();
        if (!(sort->isSorting) && !(sort->isShuffling))
        {
            if(ImGui::Button("Sort")) {
                switch(current_item)
                {
                    case 0: {
                        sort = new BubbleSort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 1: {
                        sort = new SelectionSort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 2: {
                        sort = new InsertionSort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 3: {
                        sort = new QuickSort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 4: {
                        sort = new GravitySort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 5: {
                        sort = new PigeonHoleSort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 6: {
                        sort = new RadixLSDSort(sort->elems, sort->io, setRadix);
                        goto _jmp;
                    } break;
                    case 7: {
                        sort = new CombSort(sort->elems, sort->io);
                        goto _jmp;
                    } break;
                    case 8: {
                        sort = new BogoSort(sort->elems, sort->io);
                        _jmp:
                        sort->setSpeed(setSpeed);
                        sort->setLength(setLength);
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
        ImGui::Checkbox("Reverse instead of Shuffling", &reverse);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    if(shouldSort)
    {
        shouldSort = false;
        if(!reverse)
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