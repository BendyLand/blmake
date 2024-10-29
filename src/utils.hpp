#pragma once

#include <lua.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include "os.hpp"

size_t copy_template_file(const std::string& from, const std::string& to);
bool check_table(lua_State *L, const char* tableName);
std::string get_table_commands(lua_State* L, std::string prefix);
std::string join(std::vector<std::string> vec, std::string delim);
void sanitize(std::string& original);
void ltrim(std::string& original, std::string cutset);
void rtrim(std::string& original, std::string cutset);
void trim(std::string& original, std::string cutset);
std::vector<std::string> split(std::string original, std::string delim);
std::vector<std::string> chars(std::string original);
bool starts_with_any(std::string str, std::vector<std::string> items);
bool ends_with_any(std::string str, std::vector<std::string> items);
bool is_valid_compiler(std::string compiler);
void print_valid_compilers();
void print_help_menu();
bool check_pre_build(lua_State* L);
bool ensure_executable(const char* scriptPath);
std::string extract_pre_build_path(lua_State* L);
bool validate_script_ext(std::string& path);
bool check_post_build(lua_State* L);
std::string extract_post_build_path(lua_State* L);
std::vector<std::string> split(std::string str, char delim);
std::string to_lower(std::string str);
