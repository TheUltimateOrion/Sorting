#pragma once

#include <string>

namespace Core::Platform
{

    inline static char const* BuildType()
    {
#ifdef NDEBUG
        return "Release";
#else
        return "Debug";
#endif
    }

    inline static char const* OSName()
    {
#if defined(_WIN32)
        return "Windows";
#elif defined(__APPLE__)
        return "macOS";
#elif defined(__linux__)
        return "Linux";
#else
        return "Unknown";
#endif
    }

    inline static std::string CompilerString()
    {
#if defined(__clang__)
        std::ostringstream os;
        os << "Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__;
        return os.str();
#elif defined(__GNUC__)
        std::ostringstream os;
        os << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
        return os.str();
#else
        return "Unknown compiler";
#endif
    }

    inline static char const* CppStandard()
    {
#if __cplusplus >= 202'302L
        return "C++23";
#elif __cplusplus >= 202'002L
        return "C++20";
#elif __cplusplus >= 201'703L
        return "C++17";
#elif __cplusplus >= 201'402L
        return "C++14";
#elif __cplusplus >= 201'103L
        return "C++11";
#else
        return "Pre-C++11";
#endif
    }
}  // namespace Core::Platform

