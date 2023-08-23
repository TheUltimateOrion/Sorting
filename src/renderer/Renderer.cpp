#include "renderer/Renderer.h"

void Renderer::render(Sort* sort, int a, int b, float speed)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 255, 0, 0 };
    if (!sort->sorted)
    {
        sort->last_time = (float)clock() / 1000.0f;
        textColor = { 255, 255, 255, 0 };
    }

    std::string score_text = "TIME: " + std::to_string(sort->last_time) + 's';
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, score_text.c_str(), textColor);
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
    renderGUI(sort);
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
    SDL_Delay(1 / speed);
}

void Renderer::renderGUI(Sort* sort)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Frame Time");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / sort->io.Framerate, sort->io.Framerate);
        const char* items[] = { "BubbleSort", "SelectionSort", "InsertionSort", "QuickSort", "GravitySort", "PigeonHoleSort"};

        if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(items[n], is_selected))
                    current_item = items[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }
        pressed = ImGui::Button("Sort");
        if(pressed)
            std::cout << "Hello, World!";
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}