#pragma once

#include <lua.hpp>
#include <iostream>
#include <vector>

bool check_table(lua_State *L, const char* tableName);
std::string get_table_commands(lua_State* L, std::string prefix);
std::string join(std::vector<std::string> vec, std::string delim);
