#include "utils.hpp"

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