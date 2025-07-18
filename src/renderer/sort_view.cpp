
#include "renderer/sort_view.h"

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "renderer/disp_type.h"
#include "sort/category.h"
#include "sort/sort.h"
#include "utils/common.h"
#include "utils/renderer.h"

using namespace std::literals::chrono_literals;

SortView::SortView() : m_isColored(false), m_radix(2), m_reversed(false) {}

void SortView::renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept
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

void SortView::renderInfo() const noexcept
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
    renderText(std::string("Sort: ") + AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory][AppCtx::g_app->currentItemIndex], 10, 30, { 0xFF, 0xFF, 0xFF, 0 });

    renderText("Swaps: " + std::to_string(AppCtx::g_app->sorter->swaps), 10.0f, 50.0f, { 0xFF, 0xFF, 0xFF, 0 });
    renderText("Comparisons: " + std::to_string(AppCtx::g_app->sorter->comparisons), 10.0f, 70.0f, { 0xFF, 0xFF, 0xFF, 0 });

    if (AppCtx::g_app->sorter->isSorting)
        renderText("Sorting...", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    if (AppCtx::g_app->sorter->isShuffling)
        renderText("Shuffling...", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    if (!(AppCtx::g_app->sorter->isShuffling) && !(AppCtx::g_app->sorter->isSorting) && AppCtx::g_app->sorter->sorted)
        renderText("Sorted", 10.0f, 90.0f, { 0xFF, 0xFF, 0xFF, 0 });
    SDL_SetRenderDrawColor(AppCtx::g_app->renderer, _r, _g, _b, _a);
}

void SortView::drawElement(size_t k, const RenderParams& t_params) noexcept {
    if (k >= m_elems.size()) return;

    switch (AppCtx::g_app->currentDisplayType) {
        case DisplayType::Bar: {
            SDL_FRect rect = (SDL_FRect) {k * t_params.barSpacing, AppCtx::kWinHeight, t_params.barSpacing, -(m_elems[k] * t_params.barSpacing * AppCtx::kWinHeight / AppCtx::kWinWidth)};
            SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);
        } break;

        case DisplayType::Dot: {
            SDL_RenderPoint(AppCtx::g_app->renderer, (k + 1) * t_params.barSpacing, (AppCtx::kWinHeight - m_elems[k] * t_params.barSpacing * AppCtx::kWinHeight / AppCtx::kWinWidth)); 
        } break;

        case DisplayType::RainbowRectangle: {
            m_isColored = true;
            SDL_FRect rect = (SDL_FRect){k * t_params.barSpacing, 0, t_params.barSpacing, AppCtx::kWinHeight};
            SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);
        } break;

        case DisplayType::Circle: {
            m_isColored = true;
            float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
            SDL_GetRenderDrawColorFloat(AppCtx::g_app->renderer, &r, &g, &b, &a);
                
            SDL_Vertex vertices[3];

            vertices[0] = {
                {0.5f * AppCtx::kWinWidth, 0.5f * AppCtx::kWinHeight}, /* first point location */ 
                { r, g, b, 0xFF }, /* first color */ 
                { 0.f, 0.f }
            };

            vertices[1] = {
                {0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * k), 0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * k)}, /* second point location */ 
                { r, g, b, 0xFF }, /* second color */
                { 0.f, 0.f }
            };

            vertices[2] = {
                {0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * (k + 1)), 0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * (k + 1))}, /* third point location */ 
                { r, g, b, 0xFF }, /* third color */
                { 0.f, 0.f }
            };

            if (!SDL_RenderGeometry(AppCtx::g_app->renderer, nullptr, vertices, 3, nullptr, 0))
            {
                LOGERR(SDL_GetError());
            }
        } break;

        case DisplayType::CircleDot: {
            SDL_RenderPoint(
                AppCtx::g_app->renderer, 
                0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * k), 
                0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * k)
            );
        } break;

        case DisplayType::DisparityCircle: {
            SDL_RenderLine(
                AppCtx::g_app->renderer, 
                0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * m_elems[k]), 
                0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * m_elems[k]), 
                0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * (k + 1)), 
                0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * (k + 1))
            );
        } break;

        case DisplayType::Spiral: {
            float r = 0, g = 0, b = 0, a = 0;
            SDL_GetRenderDrawColorFloat(AppCtx::g_app->renderer, &r, &g, &b, &a);
            
            SDL_Vertex vertices[3];
            
            vertices[0] = {
                {0.5f * AppCtx::kWinWidth, 0.5f * AppCtx::kWinHeight}, /* first point location */ 
                { r, g, b, 0xFF }, /* first color */ 
                { 0.f, 0.f }
            };

            vertices[1] = {
                {0.5f * AppCtx::kWinWidth + t_params.spiralScale * m_elems[k] * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * k), 0.5f * AppCtx::kWinHeight + t_params.spiralScale * m_elems[k] * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * k)}, /* second point location */ 
                { r, g, b, 0xFF }, /* second color */
                { 0.f, 0.f }
            };

            vertices[2] = {
                {0.5f * AppCtx::kWinWidth + t_params.spiralScale * m_elems[k] * cosf(t_params.degreesPerIndex * utils::kRadiansPerDegree * (k + 1)), 0.5f * AppCtx::kWinHeight + t_params.spiralScale * m_elems[k] * sinf(t_params.degreesPerIndex * utils::kRadiansPerDegree * (k + 1))}, /* third point location */ 
                { r, g, b, 0xFF }, /* third color */
                { 0.f, 0.f }
            };

            SDL_RenderGeometry(AppCtx::g_app->renderer, nullptr, vertices, 3, nullptr, 0);
        } break;

        case DisplayType::SpiralDot: {
            SDL_RenderPoint(
                AppCtx::g_app->renderer, 
                0.5f * AppCtx::kWinWidth + t_params.spiralScale * m_elems[k] * cos(t_params.degreesPerIndex * utils::kRadiansPerDegree * k), 
                0.5f * AppCtx::kWinHeight + t_params.spiralScale * m_elems[k] * sin(t_params.degreesPerIndex * utils::kRadiansPerDegree * k)
            ); 
        } break;
    }
}

void SortView::update() noexcept
{
    SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(AppCtx::g_app->renderer);

    RenderParams t_params {
        .degreesPerIndex = 360.0f / static_cast<float>(m_elems.size()),
        .barSpacing = AppCtx::kWinWidth / m_elems.size(),
        .circleRadius = std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) * 0.75 * 0.5,
        .spiralScale = 0.5f * std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) / static_cast<float>(m_elems.size())
    };

    {
        LOCK_GUARD;
        m_elems = AppCtx::g_app->data;
    }

    AppCtx::g_app->currentElement = AppCtx::g_app->sorter->getFirst();

    for (size_t k = 0; k < m_elems.size(); ++k)
    {
        SDL_Color sortColor = utils::hsvToRgb(m_elems[k] * 0xFF / m_elems.size(), 0xFF, 0xFF);

        if (k == AppCtx::g_app->sorter->getFirst() || k == AppCtx::g_app->sorter->getSecond())
            SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0xFF, 0x0, 0x0, 0xFF);
        else {
            if (m_isColored) {
                SDL_SetRenderDrawColor(AppCtx::g_app->renderer, sortColor.r, sortColor.g, sortColor.b, 0xFF);
            } else {
                SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }

        drawElement(k, t_params);
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
}

int SortView::renderGUI() noexcept
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
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / AppCtx::g_app->getFramerate(), AppCtx::g_app->getFramerate());
        
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

        if(AppCtx::g_app->currentItemIndex >= AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory].size()) AppCtx::g_app->currentItemIndex = 0;

        if (ImGui::BeginCombo("##combo2", AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory][AppCtx::g_app->currentItemIndex])) // The second parameter is the label previewed before opening the combo.
        {
            for (size_t n = 0; n < AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory].size(); ++n)
            {
                bool isSelected = (AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory][AppCtx::g_app->currentItemIndex] == AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory][n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(AppCtx::g_app->sortTypes[AppCtx::g_app->currentCategory][n], isSelected))
                    AppCtx::g_app->currentItemIndex = n;
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
        ImGui::Checkbox("Color", &m_isColored);

        ImGui::Spacing();
        ImGui::SeparatorText("Variables");
        ImGui::Spacing();

        ImGui::InputFloat("Set Speed", &BaseSort::s_speed, 0.001f);
        BaseSort::s_speed = std::clamp(BaseSort::s_speed, 0.001f, 1000.f);

        ImGui::InputInt("Set Array Length", &BaseSort::s_length, 2);
        BaseSort::s_length = std::clamp(BaseSort::s_length, 2, 1024*10);

        if (AppCtx::g_app->currentCategory == 1 && AppCtx::g_app->currentItemIndex == 0)
            ImGui::SliderInt("Set Buckets/Radix", &m_radix, 2, 10, "%d");

        ImGui::Spacing();
        if (!(AppCtx::g_app->sorter->isSorting) && !(AppCtx::g_app->sorter->isShuffling))
        {
            if(ImGui::Button("Sort")) {
                LOGINFO("Starting sort");
                switch(AppCtx::g_app->currentCategory)
                {
                    case SortCategory::Exchange: {
                        switch(AppCtx::g_app->currentItemIndex)
                        {
                            SORTCASE(0, BubbleSort);
                            SORTCASE(1, QuickSort);
                            SORTCASE(2, CombSort);
                        }
                    } break;
                    case SortCategory::Distribution: {
                        switch(AppCtx::g_app->currentItemIndex)
                        {
                            SORTCASERADIX(0, RadixLSDSort);
                            SORTCASE(1, PigeonHoleSort);
                            SORTCASE(2, GravitySort);
                            SORTCASE(3, BogoSort);
                        }
                    } break;
                    
                    case SortCategory::Insertion: {
                        switch(AppCtx::g_app->currentItemIndex) {
                            SORTCASE(0, InsertionSort);
                        }
                    } break;
                    case SortCategory::Merge: {
                        switch(AppCtx::g_app->currentItemIndex) {
                            SORTCASE(0, MergeSort);
                        }
                    } break;
                    case SortCategory::Select: {
                        switch(AppCtx::g_app->currentItemIndex) {
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
        ImGui::Checkbox("Reverse instead of Shuffling", &m_reversed);
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
            if(!m_reversed)
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