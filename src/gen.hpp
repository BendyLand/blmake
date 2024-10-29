#pragma once

#include "utils.hpp"

size_t generate_full_build();
size_t generate_build();
size_t generate_simple_build();
size_t generate_tiny_build();
size_t generate_test_build();
size_t handle_template_generation(int argc, char** argv);
int handle_cl_args(int argc, char** argv);
std::string get_lua_string(lua_State* L, const char* name);
std::vector<std::string> get_lua_string_array(lua_State* L, const char* name);
void generate_cmake_from_lua(const std::string& luaFile);
