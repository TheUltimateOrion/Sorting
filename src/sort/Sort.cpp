#include "sort/Sort.h"

Sort::Sort(std::vector<int>& arr, ImGuiIO& io) : elems(arr), sorted(false), wantBreak(false), speed(speed), io(io), last_time((float)clock() / 1000.0f) {}
void Sort::render(std::vector<int>& array, int a, int b, float speed)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 255, 0, 0 };
    if (!sorted)
    {
        last_time = (float)clock() / 1000.0f;
        textColor = { 255, 255, 255, 0 };
    }

    std::string score_text = "TIME: " + std::to_string(last_time) + 's';
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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //SDL_SetRenderDrawColor(renderer, array[k] * 255 / (LOGICAL_WIDTH - 1), 0, 255-(array[k] * 255 / (LOGICAL_WIDTH - 1)), 255);
        //SDL_RenderDrawLine(renderer, k + 1, LOGICAL_WIDTH, k + 1, LOGICAL_WIDTH - elems[k]);
        SDL_RenderDrawPoint(renderer, k + 1, LOGICAL_WIDTH - elems[k]);
    }
    this->renderGUI();
    SDL_RenderPresent(renderer);
    if(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            wantBreak = true;
            return;
        }
    }
    SDL_Delay(1 / speed);
}

void Sort::renderGUI()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Frame Time");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
void Sort::swap(std::vector<int>& array, int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}