#pragma once

#include <lua.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include "os.hpp"

size_t copy_template_file(const std::string& from, const std::string& to);
bool check_table(lua_State *L, const char* tableName);
std::string get_table_commands(lua_State* L, const std::string& prefix);
std::string join(std::vector<std::string> vec, std::string delim);
void sanitize(std::string& original);
void ltrim(std::string& original, std::string cutset);
void rtrim(std::string& original, std::string cutset);
void trim(std::string& original, std::string cutset);
std::vector<std::string> split(std::string& original, const std::string& delim);
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
std::string to_lower(std::string str);
std::string read_file(std::string path);
std::string filter_files(std::string files, std::string check);
size_t write_string_to_file(const std::string& destFilePath, const std::string& contents);
void check_error_fatal(int err, std::string message);
std::string perform_pre_build_path_extraction(lua_State* L, const char* path);
bool perform_pre_build_check(lua_State* L, const char* path);
int check_error_passive(int err, std::string message);
std::vector<std::string> filter_file_list(const std::vector<std::string>& vec);
size_t write_binary_data_to_file(const std::string& destFilePath, const unsigned char contents[], size_t size);
std::string get_lua_table_as_str(lua_State* L, const std::string& field);
std::vector<std::string> get_lua_table(lua_State* L, const std::string& field);
std::string get_lua_str(lua_State* L, const std::string& field);
std::string get_lua_table_with_cmds_as_str(lua_State* L, const std::string& field, const std::string& prefix);

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> vec)
{
    std::cout << "ITEMS:" << std::endl;
    for (T item : vec) {
        std::cout << item << std::endl;
    }
    std::cout << std::endl;
    return os;
}
