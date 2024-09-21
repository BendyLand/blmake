#include "utils.hpp"

bool validate_script_ext(std::string path)
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

std::string extract_pre_build_path(lua_State* L)
{
    std::string result = "";
    if (check_table(L, "Full_build")) {
        lua_getglobal(L, "Full_build");
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
    }
    else if (check_table(L, "Test_build")) {
        lua_getglobal(L, "Test_build");
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
    }
    return result;
}

bool check_pre_build(lua_State* L)
{
    bool result = false;
    if (check_table(L, "Full_build")) {
        lua_getglobal(L, "Full_build");
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
    }
    else if (check_table(L, "Test_build")) {
        lua_getglobal(L, "Test_build");
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
    }
    return result;
}

std::vector<std::string> split(std::string str, char delim)
{
    std::vector<std::string> result;
    std::string temp = "";
    for (char c : str) {
        if (c == delim) {
            result.push_back(temp);
            temp = "";
            continue;
        }
        temp += c;
    }
    if (temp.size() > 0) result.push_back(temp);
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

std::vector<std::string> split(std::string original, std::string delim)
{
    std::vector<std::string> result;
    std::string temp = "";
    if (delim.size() == 0) return chars(original);
    if (delim.size() == 1) {
        temp += delim;
        result.push_back(temp);
        return result;
    }
    for (size_t i = 0; i < original.size(); i++) {
        if (original[i] == delim[0]) {
            for (size_t j = 0; j < delim.size(); j++) {
                if (original[i+j] != delim[j]) goto Add;
            }
            result.push_back(temp);
            temp = "";
            continue;
        }
        Add:
        temp += original[i];
    }
    if (temp.size() > 0) result.push_back(temp);
    return result;
}

void ltrim(std::string& original, std::string cutset)
{
    std::vector<std::string> cut_chars = split(cutset, " ");
    if (starts_with_any(original, cut_chars)) {
        original.erase(0, 1);
    }
}

void rtrim(std::string& original, std::string cutset)
{
    std::vector<std::string> cut_chars = split(cutset, " ");
    while (ends_with_any(original, cut_chars)) {
        original.erase(original.size()-1, 1);
    }
}

void trim(std::string& original, std::string cutset)
{
    ltrim(original, cutset);
    rtrim(original, cutset);
}

bool check_table(lua_State *L, const char *tableName)
{
    lua_getglobal(L, tableName);
    bool isTable = lua_istable(L, -1);
    lua_pop(L, 1);
    return isTable;
}

std::string join(std::vector<std::string> vec, std::string delim)
{
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
    std::vector<std::string> valid_comps = {"gcc", "g++", "clang", "msvc"};
    for (std::string comp : valid_comps) {
        if (compiler.find(comp) != std::string::npos) return true;
    }
    return false;
}

void print_valid_compilers()
{
    std::vector<std::string> valid_comps = {"gcc", "g++", "clang", "msvc"};
    std::cout << "Valid compilers:" << std::endl;
    for (std::string comp : valid_comps) {
        std::cout << comp << std::endl;
    }
}

void print_help_menu()
{
    std::cout << \
    "Welcome to the blmake help menu!\n\n" << \
    "Usage:\nblmake <command> <args>\n\n" << \
    "The available commands are:\n" << \
    "gen <config_type> (full, build, simple, tiny, test)\nhelp\n" << std::endl;
}
