#include "renderer/Renderer.h"
#include "sort/BubbleSort.h"
#include "sort/SelectionSort.h"
#include "sort/GravitySort.h"
#include "sort/PigeonHoleSort.h"
#include "sort/InsertionSort.h"
#include "sort/QuickSort.h"

void SortRenderer::render(Sort* sort, int a, int b)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 255, 0, 0 };
    if (!sort->sorted)
    {
        sort->last_time = (float)clock() / 1000.0f - sort->start_time;
        textColor = { 255, 255, 255, 0 };
    }

    std::string timeText = "TIME: " + std::to_string(sort->last_time) + 's';
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { 10, 30, text_width, text_height };
    SDL_RenderCopy(renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);

    for (int k = 0; k < LOGICAL_WIDTH; k++)
    {
        if (k == a || k == b)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_SetRenderDrawColor(renderer, sort->elems[k] * 255 / (LOGICAL_WIDTH - 1), 0, 255-(sort->elems[k] * 255 / (LOGICAL_WIDTH - 1)), 255);
        SDL_RenderDrawLine(renderer, k + 1, LOGICAL_WIDTH, k + 1, LOGICAL_WIDTH - sort->elems[k]);
        // SDL_RenderDrawPoint(renderer, k + 1, LOGICAL_WIDTH - elems[k]);
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
    SDL_Delay(1 / sort->speed);
}

bool SortRenderer::renderGUI(Sort* sort)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    bool shouldSort = false;
    {
        ImGui::Begin("Frame Time");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / sort->io.Framerate, sort->io.Framerate);
        const char* items[] = { "BubbleSort", "SelectionSort", "InsertionSort", "QuickSort", "GravitySort", "PigeonHoleSort"};

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
        if(ImGui::Button("Sort") && sort->sorted && !(sort->isSorting)) {
            bool temp = sort->wantBreak;
            switch(current_item)
            {
                case 0: {
                    sort = new BubbleSort(sort->elems, sort->io);
                    sort->wantBreak = temp;
                    sort->setSpeed(100);
                } break;
                case 1: {
                    sort = new SelectionSort(sort->elems, sort->io);
                    sort->wantBreak = temp;
                    sort->setSpeed(0.01);
                } break;
                case 2: {
                    sort = new InsertionSort(sort->elems, sort->io);
                    sort->wantBreak = temp;
                    sort->setSpeed(5);
                } break;
                case 3: {
                    sort = new QuickSort(sort->elems, sort->io);
                    sort->wantBreak = temp;
                    sort->setSpeed(1);
                } break;
                case 4: {
                    sort = new GravitySort(sort->elems, sort->io);
                    sort->wantBreak = temp;
                    sort->setSpeed(500);
                } break;
                case 5: {
                    sort = new PigeonHoleSort(sort->elems, sort->io);
                    sort->wantBreak = temp;
                    sort->setSpeed(0.01);
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