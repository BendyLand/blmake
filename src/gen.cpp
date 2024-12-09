#include "gen.hpp"
#include "templates/blmake/full_build_template.h"
#include "templates/blmake/build_template.h"
#include "templates/blmake/simple_build_template.h"
#include "templates/blmake/tiny_build_template.h"
#include "templates/blmake/test_build_template.h"
#include "templates/premake/full_build_premake.h"
#include "templates/premake/build_premake.h"
#include "templates/premake/simple_build_premake.h"
#include "templates/premake/tiny_build_premake.h"
#include "templates/premake/test_build_premake.h"
#include "watcher_config.hpp"

// Helper function to get a string field from the Lua table
std::string get_lua_string(lua_State* L, const char* name) 
{
    lua_getfield(L, -1, name);
    std::string result = lua_tostring(L, -1);
    lua_pop(L, 1); // Pop the value off the stack
    return result;
}

// Helper function to get a table of strings from the Lua file
std::vector<std::string> get_lua_string_array(lua_State* L, const char* name) 
{
    std::vector<std::string> result;
    lua_getfield(L, -1, name); // Push table onto the stack

    if (lua_istable(L, -1)) {
        lua_pushnil(L); // Start iterating with the first key
        while (lua_next(L, -2)) {
            result.push_back(lua_tostring(L, -1));
            lua_pop(L, 1); // Pop the value, keep the key for the next iteration
        }
    }
    lua_pop(L, 1); // Pop the table from the stack
    return result;
}

size_t check_for_gen_arg(int argc, char** argv)
{
    if (argc > 1) {
        if (argv[1] == std::string("gen")) {
            size_t err = Blmake::handle_template_generation(argc, argv);
            return err;
        }
    }
    return 2;
}

size_t handle_cl_args(int argc, char** argv, lua_State* L)
{
    if (argv[1] == std::string("help")) {
        print_help_menu();
        return 0;
    }
    else if (argv[1] == std::string("align")) {
        //todo: write function below
        // align_config_comments();
        return 0;
    }
    else if (argv[1] == std::string("watch")) {
        if (argc > 2) {
            if (argv[2] == std::string("clean")) {
                clean_prev_json(L);
                return 0;
            }
        }
        size_t err = Watcher::generate_watcher_structure(L);
        return err;
    }
    else if (std::string(argv[1]).find("premake") != std::string::npos || std::string(argv[1]) == ("gen")) {
        size_t err = Premake::handle_template_generation(argc, argv, L);
        return err;
    }
    return 1;
}

namespace Premake
{
    size_t handle_template_generation(int argc, char** argv, lua_State* L)
    {
        std::string contents = generate_premake_build(L);
        std::cout << "Contents:\n" << contents << std::endl;
        // size_t err = write_string_to_file("./premake5.lua", contents);
        // if (err == 0) std::cout << "Premake file generated successfully!" << std::endl;
        size_t err = 0;
        return err;
    }

    std::string generate_premake_build(lua_State* L)
    {
        std::string contents = get_premake_contents(L);
        return contents;
    }

    std::string get_premake_contents(lua_State* L)
    {
        std::string kind = get_blmake_config_type(L);
        std::string result = "";
        if (kind == "Full_build") {
            for (size_t i = 0; i < full_build_premake_txt_len; i++) result += full_build_premake_txt[i];
        }
        else if (kind == "Build") {
            for (size_t i = 0; i < build_premake_txt_len; i++) result += build_premake_txt[i];
        }
        else if (kind == "Simple_build") {
            for (size_t i = 0; i < simple_build_premake_txt_len; i++) result += simple_build_premake_txt[i];
        }
        else if (kind == "Tiny_build") {
            for (size_t i = 0; i < tiny_build_premake_txt_len; i++) result += tiny_build_premake_txt[i];
        }
        else if (kind == "Test_build") {
            for (size_t i = 0; i < test_build_premake_txt_len; i++) result += test_build_premake_txt[i];
        }
        else {
            for (size_t i = 0; i < full_build_premake_txt_len; i++) result += full_build_premake_txt[i];
        }
        return result;
    }

/*  
-- Full_build
workspace "BendyLand" -- project_metadata.name else Full_build (table name)
configurations { "Debug", "Release" } -- constant?
location "custom_example/build" -- out_dir

project "BendyLand" -- workspace?
kind "ConsoleApp" -- constant?
language "C++" -- infer from compiler
targetdir "custom_example/build" -- out_dir
objdir "custom_example/build/obj" -- out_dir + "/obj"?

-- Compiler and files
files { "custom_example/main.cpp", "custom_example/utils.cpp" } -- src_dir + files
includedirs { "custom_example/include" } -- include_dirs

-- Preprocessor options
defines { "DEBUG" } -- preproc_opts

-- Platform-specific options
architecture "arm" -- constant w/ option to change?

-- Language standard
cppdialect "C++20" -- lang_exts if -std=c++XX

-- Build options
filter "configurations:Debug" 
    defines { "DEBUG" } 
    symbols "On"
    optimize "Off"

filter "configurations:Release"
    defines { "NDEBUG" }
    symbols "Off"
    optimize "Full"
    flags { "LinkTimeOptimization" } -- lto

-- Warnings and flags
buildoptions {
    "-Wall", "-Werror", "-pg", "-flto", "-std=c++20"
} -- warnings, profiling, lto, lang_exts

-- Linker options
libdirs { "custom_example/libs" } -- linker_opts
links { "m", "pthread" } -- dependencies

-- Custom scripts/hooks
prebuildcommands { 
    "custom_example/scripts/pre_build.sh"
} -- hooks.pre_build

postbuildcommands {
    "custom_example/scripts/post_build.sh"
} -- hooks.post_build

-- Additional flags
buildoptions { "-fno-stack-protector" } -- custom_flags
*/
};

namespace Blmake
{
    size_t generate_full_build()
    {
        std::string contents = ""; 
        for (size_t i = 0; i < full_build_template_txt_len; i++) contents += full_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_build()
    {
        std::string contents = ""; 
        for (size_t i = 0; i < build_template_txt_len; i++) contents += build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_simple_build()
    {
        std::string contents = ""; 
        for (size_t i = 0; i < simple_build_template_txt_len; i++) contents += simple_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_tiny_build()
    {
        std::string contents = ""; 
        for (size_t i = 0; i < tiny_build_template_txt_len; i++) contents += tiny_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_test_build()
    {
        std::string contents = ""; 
        for (size_t i = 0; i < test_build_template_txt_len; i++) contents += test_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t handle_template_generation(int argc, char** argv)
    {
        if (argc > 2) {
            std::string check = to_lower(argv[2]);
            if (check == std::string("full")) {
                std::cout << "Generating Full_build in blmake.lua..." << std::endl;
                size_t err = generate_full_build();
                if (err) exit(1);
            }
            else if (check == std::string("build")) {
                std::cout << "Generating Build in blmake.lua..." << std::endl;
                size_t err = generate_build();
                if (err) exit(1);
            }
            else if (check == std::string("simple")) {
                std::cout << "Generating Simple_build in blmake.lua..." << std::endl;
                size_t err = generate_simple_build();
                if (err) exit(1);
            }
            else if (check == std::string("tiny")) {
                std::cout << "Generating Tiny_build in blmake.lua..." << std::endl;
                size_t err = generate_tiny_build();
                if (err) exit(1);
            }
            else if (check == std::string("test")) {
                std::cout << "Generating Test_build in blmake.lua..." << std::endl;
                size_t err = generate_test_build();
                if (err) exit(1);
            }
            else if (check == std::string("premake")) {
                return 2;
            }
            else {
                std::cerr << "Unknown template type." << std::endl;
                return 1;
            }
        }
        else {
            std::cout << "Generating Full_build in blmake.lua..." << std::endl;
            size_t err = generate_full_build();
            if (err) exit(1);
        }
        return 0;
    }
};

namespace Watcher 
{
    size_t generate_watcher_structure(lua_State* L)
    {
        std::string watcher_binary = (char*)src_watcher_watcher;
        std::string prev_json = reinterpret_cast<const char*>(src_watcher_prev_json);
        std::string src_path = "";
        std::string config_type = get_blmake_config_type(L);
        lua_getglobal(L, config_type.c_str());
        lua_getfield(L, -1, "src_dir");
        if (lua_isstring(L, -1)) {
            std::string src_dir = lua_tostring(L, -1);
            sanitize(src_dir);
            if (!src_dir.empty()) {
                src_path += src_dir + "/";
            }
        }
        lua_pop(L, 1);
        src_path += "watcher";
        std::filesystem::create_directory(src_path);
    #if OS_UNIX_LIKE_DEFINED
        size_t err1 = write_binary_data_to_file(src_path + "/watcher", src_watcher_watcher, src_watcher_watcher_len);
        std::string permissions_cmd = "chmod +x " + src_path + "/watcher";
        OS::run_command(permissions_cmd);
    #else 
        size_t err1 = 0; //! temporary until Windows implementation works
    #endif
        size_t err2 = write_string_to_file(src_path + "/prev.json", prev_json);
        size_t err3 = write_string_to_file(src_path + "/recompile_list.txt", "");
        size_t any_err = err1 | err2 | err3; 
        if (any_err == 0) std::cout << "Watcher structure generated successfully!" << std::endl;
        return any_err;
    }
};