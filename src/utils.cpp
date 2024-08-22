#include "utils.hpp"

bool check_table(lua_State *L, const char *tableName)
{
    lua_getglobal(L, tableName);
    bool isTable = lua_istable(L, -1);
    lua_pop(L, 1);
    return isTable;
}
