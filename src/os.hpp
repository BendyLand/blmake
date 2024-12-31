#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <cstring>

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

#if defined(OS_LINUX)
    #include <sys/wait.h>
    #include <string.h>
    #include <filesystem>
    #ifndef SIZE_T_MAX
        #define SIZE_T_MAX SIZE_MAX
    #endif
#endif

#if defined(OS_MACOS) || defined(OS_LINUX) || defined(OS_UNIX) || defined(OS_FREEBSD)
    #define OS_UNIX_LIKE
#endif 

#if defined(OS_WINDOWS)
    #define WIN32_LEAN_AND_MEAN 
    #include <Windows.h>
#endif

#ifndef OS_UNIX_LIKE_DEFINED
    #if defined(OS_UNIX_LIKE)
        #define OS_UNIX_LIKE_DEFINED 1
    #else
        #define OS_UNIX_LIKE_DEFINED 0
    #endif
#endif

#ifndef OS_WINDOWS_DEFINED
    #if defined(OS_WINDOWS)
        #define OS_WINDOWS_DEFINED 1
    #else
        #define OS_WINDOWS_DEFINED 0
    #endif
#endif

class OS
{
public:
    static std::string detect_os();
    static std::pair<int, std::string> run_command(std::string& args);
private: 
    static std::pair<int, std::string> run_command_unix(const std::vector<std::string>& args);
    static std::pair<int, std::string> run_command_windows(const std::string& command);
};

inline std::vector<std::string> os_chars(std::string original)
{
    std::vector<std::string> result;
    std::string temp = "";
    for (char c : original) {
        temp += c;
        result.push_back(temp);
        temp = "";
    }
    return result;
}

inline std::vector<std::string> os_split(std::string& original, const std::string& delim)
{
    std::vector<std::string> result;
    std::string temp = "";
    std::string copy = original;
    if (delim.size() == 0) return os_chars(original);
    if (original.size() <= 1) {
        result.emplace_back(original);
        return result;
    }
    while (copy.find(delim) != std::string::npos) {
        temp = copy.substr(0, copy.find(delim));
        result.emplace_back(temp);
        copy.erase(0, copy.find(delim) + delim.size());
    }
    if (copy.size() > 0) result.emplace_back(copy);
    if (result.size() == 0) result.emplace_back(original);
    return result;
}
