#pragma once

#include <iostream>
#include "utils.hpp"

std::string construct_full_build_command(lua_State* L);
std::string construct_build_command(lua_State* L);
std::string construct_simple_build_command(lua_State* L);
std::string construct_tiny_build_command(lua_State* L);
std::string construct_test_build_command(lua_State* L);
std::string handle_command_construction(lua_State* L);