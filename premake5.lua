-- premake5.lua generated from the Test_build table in blmake.lua!

workspace "Test_build"
   configurations { "Debug", "Release" }
   location "custom_example/build"

project "Test_build"
   kind "ConsoleApp"
   language "C++"
   targetdir "custom_example/build"
   objdir "custom_example/build/obj"

   files { "custom_example/main.cpp", "custom_example/utils.cpp" }
   includedirs { "custom_example/include" }
   libdirs { "custom_example/libs" }
   links { "m", "pthread" }

   -- Preprocessor, warnings, and profiling
   defines { "DEBUG" }
   buildoptions { "-Wall", "-Werror", "-pg" }

   -- Debug and release filters
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      symbols "Off"
      optimize "Full"

   -- Custom hooks
   prebuildcommands { "custom_example/scripts/pre_build.sh" }
   postbuildcommands { "custom_example/scripts/post_build.sh" }