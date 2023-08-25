#include "main.h"
#include "renderer/Renderer.h"
#include "sort/BubbleSort.h"
#include "sort/SelectionSort.h"
#include "sort/GravitySort.h"
#include "sort/PigeonHoleSort.h"
#include "sort/InsertionSort.h"
#include "sort/QuickSort.h"
#include "sort/RadixLSDSort.h"

static float setSpeed = 1.0f;
static bool isColored = false;
static bool isDot = false;
static const char *items[] = { "BubbleSort", "SelectionSort", "InsertionSort", "QuickSort", "GravitySort", "PigeonHoleSort", "RadixLSDSort"};
static int current_item = 0;
static bool isRadix = false;
static int setRadix = 2;

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

void SortRenderer::render(Sort* sort, std::vector<int>& elems, int a, int b)
{
    ::calculateDeltaTime();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 255, 0, 0 };
    if (sort->isSorting)
        ::last_time = (float)clock() / 1000.0f - sort->start_time;
    if (sort->isSorting || (::last_time == 0.0f))
        textColor = { 255, 255, 255, 0 };
    
    renderText("TIME: " + std::to_string(::last_time) + 's', 10, 30, textColor);
    renderText(std::string("Sort: ") + items[current_item], 10, 50, { 255, 255, 255, 0 });
    if (sort->isSorting)
        renderText("Sorting...", 10, 70, { 255, 255, 255, 0 });
    if (sort->isShuffling)
        renderText("Shuffling...", 10, 70, { 255, 255, 255, 0 });
    if (!(sort->isShuffling) && !(sort->isSorting) && sort->sorted)
        renderText("Sorted", 10, 70, { 255, 255, 255, 0 });

    for (int k = 0; k < LOGICAL_WIDTH; k++)
    {
        if (k == a || k == b)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            if (isColored)
                SDL_SetRenderDrawColor(renderer, elems[k] * 255 / (LOGICAL_WIDTH - 1), 0, 255-(elems[k] * 255 / (LOGICAL_WIDTH - 1)), 255);
        
        if (isDot)
            SDL_RenderDrawPoint(renderer, k + 1, LOGICAL_WIDTH - elems[k]);
        else
            SDL_RenderDrawLine(renderer, k + 1, LOGICAL_WIDTH, k + 1, LOGICAL_WIDTH - elems[k]);
    }
    if(renderGUI(sort))
        return;
    SDL_RenderPresent(renderer);
    if(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            sort->wantBreak = true;
            return;
        }
    }
    SDL_Delay(1 / (sort->speed * deltaTime));
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
        ImGui::Checkbox("Color", &isColored);
        ImGui::SameLine();
        ImGui::Checkbox("Dot or line", &isDot);
        ImGui::InputFloat("Set Speed", &setSpeed, 0.05f);
        if (current_item == 6)
            ImGui::SliderInt("Set Buckets/Radix", &setRadix, 2, 10, "%d");
        if(ImGui::Button("Sort") && sort->sorted && !(sort->isSorting)) {
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
                    _jmp:
                    sort->setSpeed(setSpeed);
                } break;
                default:
                    std::cout << "Invalid Sort!" << std::endl;
            }
            shouldSort = true;
        }
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    if(shouldSort)
    {
        shouldSort = false;
        sort->shuffle();
        sort->sort();
        if (sort->wantBreak)
            return 1;
    }
    return 0;
}