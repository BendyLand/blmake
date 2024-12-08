#pragma once

#include "utils.hpp"
#include "build.hpp"
#include "os.hpp"

size_t handle_cl_args(int argc, char** argv, lua_State* L);
size_t check_for_gen_arg(int argc, char** argv);
std::string get_lua_string(lua_State* L, const char* name);
std::vector<std::string> get_lua_string_array(lua_State* L, const char* name);

namespace Premake
{
    size_t handle_template_generation(int argc, char** argv, lua_State* L);
    std::string generate_premake_build(lua_State* L);
    std::string get_premake_contents(lua_State* L);
};

namespace Watcher
{
    size_t generate_watcher_structure(lua_State* L);
};

namespace Blmake
{
    size_t generate_full_build();
    size_t generate_build();
    size_t generate_simple_build();
    size_t generate_tiny_build();
    size_t generate_test_build();
    size_t handle_template_generation(int argc, char** argv);
};
