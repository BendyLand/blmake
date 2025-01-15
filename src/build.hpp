#pragma once

#include <iostream>
#include <array>
#include <filesystem>
#include "utils.hpp"
#include "os.hpp"

std::string construct_full_build_command(lua_State* L);
std::vector<std::string> construct_incremental_full_build_commands(lua_State* L);
std::string construct_build_command(lua_State* L);
std::string construct_simple_build_command(lua_State* L);
std::string construct_tiny_build_command(lua_State* L);
std::string construct_test_build_command(lua_State* L);
std::vector<std::string> handle_incremental_command_construction(lua_State* L);
std::string handle_command_construction(lua_State* L);
std::string get_blmake_config_type(lua_State* L);
std::string get_config_value(lua_State* L, std::string field);
void run_pre_build_script(lua_State* L);
void run_post_build_script(lua_State* L);
void clean_prev_json(lua_State* L);
