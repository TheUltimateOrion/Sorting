#pragma once

#include "core/timer.h"

#include <sstream>
#include <string>

namespace Core
{

    enum class LogLevel
    {
        Info,
        Warn,
        Error,
    };

    namespace Logger
    {
        void Log(LogLevel level, std::string const& message);
    }

}  // namespace Core

#define LOG_INFO(msg)                                        \
    do {                                                     \
        std::ostringstream _oss;                             \
        _oss << msg;                                         \
        Core::Logger::Log(Core::LogLevel::Info, _oss.str()); \
    }                                                        \
    while (0)

#define LOG_WARN(msg)                                        \
    do {                                                     \
        std::ostringstream _oss;                             \
        _oss << msg;                                         \
        Core::Logger::Log(Core::LogLevel::Warn, _oss.str()); \
    }                                                        \
    while (0)

#define LOG_ERROR(msg)                                        \
    do {                                                      \
        std::ostringstream _oss;                              \
        _oss << msg;                                          \
        Core::Logger::Log(Core::LogLevel::Error, _oss.str()); \
    }                                                         \
    while (0)
