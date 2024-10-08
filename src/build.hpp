#pragma once

#include <iostream>
#include "utils.hpp"
#include "os.hpp"

std::string construct_full_build_command(lua_State* L);
std::string construct_build_command(lua_State* L);
std::string construct_simple_build_command(lua_State* L);
std::string construct_tiny_build_command(lua_State* L);
std::string construct_test_build_command(lua_State* L);
std::string handle_command_construction(lua_State* L);
void run_pre_build_script(lua_State* L);
