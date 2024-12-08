workspace "BendyLand"
   configurations { "Debug", "Release" }
   location "custom_example/build"

project "BendyLand"
   kind "ConsoleApp"
   language "C++"
   targetdir "custom_example/build"
   objdir "custom_example/build/obj"

   -- Compiler and files
   files { "custom_example/main.cpp", "custom_example/utils.cpp" }
   includedirs { "custom_example/include" }

   -- Preprocessor options
   defines { "DEBUG" }

   -- Platform-specific options
   architecture "arm"

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
   buildoptions {
      "-Wall", "-Werror", "-pg", "-flto", "-std=c++20"
   }

   -- Linker options
   libdirs { "custom_example/libs" }
   links { "m", "pthread" }

   -- Custom scripts/hooks
   prebuildcommands {
      "custom_example/scripts/pre_build.sh"
   }

   postbuildcommands {
      "custom_example/scripts/post_build.sh"
   }

   -- Custom environment variables (if needed)
   -- os.setenv("CC", "gcc")
   -- os.setenv("CXX", "g++")

   -- Additional flags
   -- buildoptions { "-fno-stack-protector" }
