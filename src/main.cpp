#include <lua.hpp>
#include <iostream>

int main() 
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Load and run the Lua configuration file
    if (luaL_loadfile(L, "src/script.lua") != LUA_OK || lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Failed to load config: " << lua_tostring(L, -1) << std::endl;
        return 1;
    }

    lua_getglobal(L, "simple_build");

    if (lua_istable(L, -1)) {
        // Access specific fields
        lua_getfield(L, -1, "compiler");
        const char* compiler = lua_tostring(L, -1);
        std::cout << "Compiler: " << compiler << std::endl;
        lua_pop(L, 1);

        lua_getfield(L, -1, "lang_exts");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                const char* ext = lua_tostring(L, -1);
                std::cout << "Language extension: " << ext << std::endl;
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "output");
        const char* output = lua_tostring(L, -1);
        std::cout << "Output: " << output << std::endl;
        lua_pop(L, 1);

        // Example of iterating over 'files' table
        lua_getfield(L, -1, "files");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);  // First key
            while (lua_next(L, -2) != 0) {
                const char* file = lua_tostring(L, -1);
                std::cout << "File: " << file << std::endl;
                lua_pop(L, 1);  // Pop the value, keep the key for the next iteration
            }
        }
        lua_pop(L, 1);  // Pop the 'files' table
    }
	else {
		std::cout << "I'm in the else branch" << std::endl;
	}

    lua_close(L);
    return 0;
}

