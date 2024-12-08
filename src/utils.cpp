#include "utils.hpp"

int check_error_passive(int err, std::string message)
{
    if (err) {
        std::cerr << message << std::endl;
    }
    return err;
}

void check_error_fatal(int err, std::string message)
{
    if (err) {
        std::cerr << message << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string filter_files(std::string files, std::string check)
{
    std::vector<std::string> originals = split(files, " ");
    std::vector<std::string> check_files = split(check, "\n");
    std::vector<std::string> result;
    result.reserve(originals.size());
    for (std::string original : originals) {
        for (std::string check : check_files) {
            if (original == check) result.emplace_back(original);
        }
    }
    
    return join(result, " ");
}

std::string read_file(std::string path)
{
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

bool validate_script_ext(std::string& path)
{
    bool result = false;
    std::vector<std::string> valid_exts = {".sh", ".ps1", ".rb", ".py", ".lua"};
    sanitize(path);
    if (ends_with_any(path, valid_exts)) result = true;
    return result;
}

bool ensure_executable(const char* scriptPath)
{
    #if defined(OS_UNIX_LIKE)
        // Check if the file is executable
        if (access(scriptPath, X_OK) == 0) {
            return true;
        }
        // File is not executable, attempt to add executable permission
        if (chmod(scriptPath, S_IRWXU) == 0) {
            return true;
        }
        std::cerr << "Failed to make the script executable." << std::endl;
        return false;
    #else
        std::cout << "Windows command execution not yet supported." << std::endl;
        return false;
    #endif
}

std::string perform_pre_build_path_extraction(lua_State* L, const char* path)
{
    std::string result = "";
    lua_getglobal(L, path);
    lua_getfield(L, -1, "hooks");
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            // Check if the key is a string
            if (lua_isstring(L, -2)) {
                const char* key = lua_tostring(L, -2);
                if (strcmp(key, "pre_build") == 0) {
                    if (lua_isstring(L, -1)) {
                        result = lua_tostring(L, -1);
                    }
                } 
            }
            lua_pop(L, 1); // Pop value, keep key for next iteration
        }
    }
    return result;
}

std::string extract_pre_build_path(lua_State* L)
{
    std::string result = "";
    if (check_table(L, "Full_build")) {
        result = perform_pre_build_path_extraction(L, "Full_build");
    }
    else if (check_table(L, "Build")) {
        result = perform_pre_build_path_extraction(L, "Build");
    }
    else if (check_table(L, "Simple_build")) {
        result = perform_pre_build_path_extraction(L, "Simple_Build");
    }
    else if (check_table(L, "Test_build")) {
        result = perform_pre_build_path_extraction(L, "Test_build");
    }
    return result;
}

bool perform_pre_build_check(lua_State* L, const char* path)
{
    bool result = false;
    lua_getglobal(L, path);
    lua_getfield(L, -1, "hooks");
    if (lua_istable(L, -1)) {
        std::string path = "";
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            // Check if the key is a string
            if (lua_isstring(L, -2)) {
                const char* key = lua_tostring(L, -2);
                if (strcmp(key, "pre_build") == 0) {
                    result = true;
                } 
            }
            lua_pop(L, 1); // Pop value, keep key for next iteration
        }
    }
    return result;
}

bool check_pre_build(lua_State* L)
{
    bool result = false;
    if (check_table(L, "Full_build")) {
        result = perform_pre_build_check(L, "Full_build");
    }
    else if (check_table(L, "Build")) {
        result = perform_pre_build_check(L, "Build");
    }
    else if (check_table(L, "Simple_build")) {
        result = perform_pre_build_check(L, "Simple_build");
    }
    else if (check_table(L, "Test_build")) {
        result = perform_pre_build_check(L, "Test_build");
    }
    return result;
}

std::string perform_post_build_path_extraction(lua_State* L, const char* path)
{
    std::string result = "";
    lua_getglobal(L, path);
    lua_getfield(L, -1, "hooks");
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            // Check if the key is a string
            if (lua_isstring(L, -2)) {
                const char* key = lua_tostring(L, -2);
                if (strcmp(key, "post_build") == 0) {
                    if (lua_isstring(L, -1)) {
                        result = lua_tostring(L, -1);
                    }
                } 
            }
            lua_pop(L, 1); // Pop value, keep key for next iteration
        }
    }
    return result;
}

std::string extract_post_build_path(lua_State* L)
{
    std::string result = "";
    if (check_table(L, "Full_build")) {
        result = perform_post_build_path_extraction(L, "Full_build");
    }
    else if (check_table(L, "Build")) {
        result = perform_post_build_path_extraction(L, "Build");
    }
    else if (check_table(L, "Simple_build")) {
        result = perform_post_build_path_extraction(L, "Simple_build");
    }
    else if (check_table(L, "Test_build")) {
        result = perform_post_build_path_extraction(L, "Test, build");
    }
    return result;
}

bool perform_post_build_check(lua_State* L, const char* path)
{
    bool result = false;
    lua_getglobal(L, path);
    lua_getfield(L, -1, "hooks");
    if (lua_istable(L, -1)) {
        std::string path = "";
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            // Check if the key is a string
            if (lua_isstring(L, -2)) {
                const char* key = lua_tostring(L, -2);
                if (strcmp(key, "post_build") == 0) {
                    result = true;
                } 
            }
            lua_pop(L, 1); // Pop value, keep key for next iteration
        }
    }
    return result;
}

bool check_post_build(lua_State* L)
{
    bool result = false;
    if (check_table(L, "Full_build")) {
        result = perform_post_build_check(L, "Full_build");
    }
    else if (check_table(L, "Build")) {
        result = perform_post_build_check(L, "Build");
    }
    else if (check_table(L, "Simple_build")) {
        result = perform_post_build_check(L, "Simple_build");
    }
    else if (check_table(L, "Test_build")) {
        result = perform_post_build_check(L, "Test_build");
    }
    return result;
}

size_t copy_template_file(const std::string& templateFilePath, const std::string& newFilePath)
{
    std::ifstream templateFile(templateFilePath);
    if (!templateFile.is_open()) {
        std::cerr << "Error opening template file: " << templateFilePath << std::endl;
        return 1;
    }
    std::string fileContents((std::istreambuf_iterator<char>(templateFile)), std::istreambuf_iterator<char>());
    templateFile.close();
    std::ofstream newFile(newFilePath);
    if (!newFile.is_open()) {
        std::cerr << "Error creating new file: " << newFilePath << std::endl;
        return 1;
    }
    newFile << fileContents;
    newFile.close();
    return 0;
}

size_t write_string_to_file(const std::string& destFilePath, const std::string& contents)
{
    std::ofstream newFile(destFilePath);
    if (!newFile.is_open()) {
        std::cerr << "Error creating new file: " << destFilePath << std::endl;
        return 1;
    }
    newFile << contents;
    newFile.close();
    return 0;
}

size_t write_binary_data_to_file(const std::string& destFilePath, const unsigned char contents[], size_t size)
{
    std::ofstream newFile(destFilePath);
    if (!newFile.is_open()) {
        std::cerr << "Error creating new file: " << destFilePath << std::endl;
        return 1;
    }
    for (size_t i = 0; i < size; i++) newFile << contents[i];
    newFile.close();
    return 0;
}

std::string to_lower(std::string str)
{
    std::string result = "";
    for (char c : str) {
        result += tolower(c);
    }
    return result;
}

bool starts_with_any(std::string str, std::vector<std::string> items)
{
    for (std::string item : items) {
        if (str.starts_with(item)) return true;
    }
    return false;
}

bool ends_with_any(std::string str, std::vector<std::string> items)
{
    for (std::string item : items) {
        if (str.ends_with(item)) return true;
    }
    return false;
}

std::vector<std::string> chars(std::string original)
{
    std::vector<std::string> result;
    std::string temp = "";
    for (char c : original) {
        temp += c;
        result.push_back(temp);
        temp = "";
    }
    return result;
}

std::vector<std::string> split(std::string& original, const std::string& delim)
{
    std::vector<std::string> result;
    std::string temp = "";
    std::string copy = original;
    if (delim.size() == 0) return chars(original);
    if (original.size() <= 1) {
        result.emplace_back(original);
        return result;
    }
    while (copy.find(delim) != std::string::npos) {
        temp = copy.substr(0, copy.find(delim));
        result.emplace_back(temp);
        copy.erase(0, copy.find(delim) + delim.size());
    }
    if (copy.size() > 0) result.emplace_back(copy);
    if (result.size() == 0) result.emplace_back(original);
    return result;
}

void ltrim(std::string& original, std::string cutset)
{
    std::vector<std::string> cut_chars = split(cutset, "");
    if (starts_with_any(original, cut_chars)) {
        original.erase(0, 1);
    }
}

void rtrim(std::string& original, std::string cutset)
{
    std::vector<std::string> cut_chars = split(cutset, "");
    while (ends_with_any(original, cut_chars)) {
        original.erase(original.size()-1, 1);
    }
}

void trim(std::string& original, std::string cutset)
{
    ltrim(original, cutset);
    rtrim(original, cutset);
}

bool check_table(lua_State* L, const char* tableName)
{
    lua_getglobal(L, tableName);
    bool isTable = lua_istable(L, -1);
    lua_pop(L, 1);
    return isTable;
}

std::string join(std::vector<std::string> vec, std::string delim)
{
    if (vec.size() == 0) return "";
    if (vec.size() == 1) return vec[0];
    std::string result = "";
    int len = vec.size();
    for (int i = 0; i < len; i++) {
        result += vec[i];
        if (i < len-1) {
            result += delim;
        }
    }
    return result;
}

std::string get_table_commands(lua_State* L, std::string prefix)
{
    std::string result = "";
    if (lua_istable(L, -1)) {
        std::vector<std::string> cmd;
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            if (lua_isstring(L, -1)) {
                std::string temp = prefix + lua_tostring(L, -1);
                cmd.push_back(temp);
            }
            lua_pop(L, 1); // Pop value, keep key for next iteration
        }
        result = join(cmd, " ");
    }
    return result;
}

void sanitize(std::string& original)
{
    std::string sanitized;
    for (char c : original) {
        if (isalnum(c) || c == '.' || c == '/' || c == '-' || c == '_' || c == '+' || c == ' ' || c == '=') {
            sanitized += c;
        }
    }
    trim(sanitized, " ");
    original = sanitized;
}

bool is_valid_compiler(std::string compiler)
{
    std::vector<std::string> valid_comps = {"gcc", "g++", "clang", "msvc", "tcc"};
    for (std::string comp : valid_comps) {
        if (compiler.find(comp) != std::string::npos) return true;
    }
    return false;
}

void print_valid_compilers()
{
    std::vector<std::string> valid_comps = {"gcc", "g++", "clang", "msvc", "tcc"};
    std::cout << "Valid compilers:" << std::endl;
    for (std::string comp : valid_comps) {
        std::cout << comp << std::endl;
    }
}

void print_help_menu()
{
    std::cout << "\n" << \
    "Welcome to the blmake help menu!\n\n" << \
    "Usage:\nblmake <command> <args>\n\n" << \
    "The available commands are:\n" << \
    "gen <config_type> (full, build, simple, tiny, test)\nhelp\n" << std::endl;
}

std::vector<std::string> filter_file_list(const std::vector<std::string>& vec)
{
    std::vector<std::string> result;
    for (std::string entry : vec) {
        for (char c : entry) {
            if (!isspace(c)) {
                result.emplace_back(entry);
                goto Next;
            }
        }
        Next:
    }
    return result;
}