-- premake5.lua generated from the Simple_build table in blmake.lua!

workspace "Simple_build"
   configurations { "Default" }
   location "src"

project "Simple_build"
   kind "ConsoleApp"
   language "C++"
   targetdir "./"
   objdir "./obj"

   files { "main.cpp", "utils.cpp", "build.cpp", "os.cpp", "gen.cpp" }
   includedirs { "" }

   -- Debug settings
   filter "configurations:Default"
      symbols "Off"