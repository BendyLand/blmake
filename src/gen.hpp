#pragma once

#include <unordered_map>
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
    std::string populate_premake_template(lua_State* L, const std::string& contents);
    std::string populate_test_build_premake_template(lua_State* L, const std::string contents);
    std::string populate_tiny_build_premake_template(lua_State* L, const std::string& contents);
    std::string populate_simple_build_premake_template(lua_State* L, const std::string& contents);
    std::string populate_build_premake_template(lua_State* L, const std::string& contents);
    void insert_field_str(std::vector<std::string>*& vec, const std::string& field, std::string& value);
    void insert_field_tbl(std::vector<std::string>*& vec, const std::string& field, std::string& value);
    std::string populate_full_build_premake_template(lua_State* L, const std::string& contents);
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
