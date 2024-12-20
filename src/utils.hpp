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

std::string replace_all(std::string& str, const std::string& from, const std::string& to);
void add_all_leading_tabs(std::string& str);
void remove_space_after_slash(std::string& str);
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
std::string add_quotes(const std::string& str);
std::string remove_extra_spaces(const std::string& str);
std::string add_quotes_and_commas(std::string& str);
std::string to_lowercase(const std::string& str);
std::string to_uppercase(const std::string& str);
size_t search(const std::vector<std::string>& vec, const std::string& item);

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

template <typename T>
bool contains(std::vector<T> vec, T item)
{
    for (T entry : vec) {
        if (entry == item) return true;
    }
    return false;
}

/** 
 * Searches the FIRST 10 CHARS of each entry for the occurrence of a substring.
 * For a full text search, use the `search` function.
 * @param vec The vector to look through.
 * @param item The substring to look for.
 * @returns The first entry of `vec` which contains `item` in the first 10 chars.
 */
size_t find(const std::vector<std::string>& vec, const std::string& item);

template <typename T>
size_t find(std::vector<T> vec, T item)
{
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == item) return i;
    }
    return SIZE_T_MAX;
}
