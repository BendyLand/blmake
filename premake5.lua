-- premake5.lua generated from the Full_build table in blmake.lua!

workspace "Full_build" 
configurations { "Debug", "Release" } 
location "build" 

project "Full_build" 
kind "ConsoleApp" 
language "C++" 
targetdir "build" 
objdir "build/obj" 

-- Compiler and files
files { "main.cpp", "utils.cpp" } 
includedirs { "include" } 

-- Preprocessor options
defines { "DEBUG" } 

-- Language standard
cppdialect "C++20" 

-- Build options
filter "configurations:Debug" 
    defines { "DEBUG" } 
    symbols "On"
    optimize "Off"

filter "configurations:Release"
    defines { "NDEBUG" } 
    symbols "Off"
    optimize "Full" 
    flags { "LinkTimeOptimization" } 

-- Warnings and flags
buildoptions { "-Wall", "-Werror", "-flto", "-pg" } 

-- Linker options
libdirs { "libs" } 
links { "m", "pthread" } 

-- Custom scripts/hooks
prebuildcommands { "scripts/pre_build.sh" } 

postbuildcommands { "scripts/post_build.sh" } 