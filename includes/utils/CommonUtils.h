#pragma once

#ifndef TESTING
#define LOCK_GUARD std::lock_guard<std::mutex> lock(AppCtx::g_app->m_mutex)
#else
#define LOCK_GUARD do {} while(0);
#endif

#ifndef STYLESET
#define STYLESET(param) t_style.Colors[ImGuiCol_##param]
#endif

namespace Utils {
    
};