#pragma once

#include <iostream>
#include <unistd.h>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MACOS
#elif defined(__linux__)
    #define OS_LINUX
#elif defined(__FreeBSD__)
    #define OS_FREEBSD
#elif defined(__unix__)
    #define OS_UNIX
#else
    #define OS_UNKNOWN
#endif

#if defined(OS_MACOS) || defined(OS_LINUX) || defined(OS_UNIX) || defined(OS_FREEBSD)
    #define OS_UNIX_LIKE
#endif 

#if defined(OS_WINDOWS)
    #define WIN32_LEAN_AND_MEAN 
    #include <Windows.h>
#endif

namespace os_specific 
{
    std::string detect_os();
    int run_command_unix(const std::vector<std::string>& args);
    int run_command(const std::vector<std::string>& args);
}