#include "utils/common.h"

#ifdef __GNUG__
    #include <cxxabi.h>
#endif

namespace Utils
{
    void terminateThread(std::optional<std::thread>& t_thread)
    {
        if (t_thread.has_value())
        {
            if (t_thread->joinable()) { t_thread->join(); }

            t_thread.reset();
        }
    }

    std::string demangleName(char const* name)
    {
#ifdef __GNUG__
        int         status    = 0;
        char*       demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string result    = (status == 0 && demangled != nullptr) ? demangled : name;

        delete demangled;

        return result;
#else
        return name;  // MSVC already demangled
#endif
    }
}  // namespace Utils
