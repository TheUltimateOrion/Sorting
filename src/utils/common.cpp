#include "utils/common.h"

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
} // namespace Utils