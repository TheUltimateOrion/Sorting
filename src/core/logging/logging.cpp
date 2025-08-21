#include "core/logging/logging.h"

#include <iostream>
#include <thread>

#ifdef __ANDROID__
    #include <android/log.h>
#endif

namespace Core::Logger
{

    void Log(LogLevel level, std::string const& message)
    {
#ifdef __ANDROID__
        int priority = ANDROID_LOG_DEBUG;
        switch (level)
        {
            case LogLevel::Info : priority = ANDROID_LOG_INFO; break;
            case LogLevel::Warn : priority = ANDROID_LOG_WARN; break;
            case LogLevel::Error: priority = ANDROID_LOG_ERROR; break;
        }
        std::ostringstream idStream;
        idStream << std::this_thread::get_id();
        __android_log_print(
            priority, "OrionSort", "[Thread ID: %s][%fs]: %s", idStream.str().c_str(),
            Timer::GetTimestamp(), message.c_str()
        );
#else
        std::ostream* out      = &std::cout;
        char const*   levelStr = "INFO";
        switch (level)
        {
            case LogLevel::Info:
                out      = &std::cout;
                levelStr = "INFO";
                break;
            case LogLevel::Warn:
                out      = &std::cerr;
                levelStr = "WARN";
                break;
            case LogLevel::Error:
                out      = &std::cerr;
                levelStr = "ERROR";
                break;
        }
        (*out) << "[" << levelStr << "][Thread ID: " << std::this_thread::get_id() << "]["
               << Timer::GetTimestamp() << "s]: " << message << std::endl;
#endif
    }

}  // namespace Core::Logger
