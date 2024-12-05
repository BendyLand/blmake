#include "utils.hpp"
#include "build.hpp"
#include "os.hpp"
#include "gen.hpp"

//todo: figure out how to get incremental builds into out_dir
int main(int argc, char** argv)
{
    // Check for command line args
    if (argc > 1) {
        int err = handle_cl_args(argc, argv);
        check_error_fatal(err, "Error handling command line arguments.");
        return 0;
    }

    // Create new Lua state for the config file to use
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Load and run the Lua config file
    //! Eventually update this to be ./blmake.lua
    if (luaL_loadfile(L, "src/blmake.lua") != LUA_OK || lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Failed to load config: " << lua_tostring(L, -1) << std::endl;
        return 1;
    }

    // Handle pre-build scripts
    if (check_pre_build(L)) run_pre_build_script(L);

    // Construct and run compilation command
    // std::vector<std::string> commands = handle_command_construction(L);
    std::string command = handle_command_construction(L);
    // if (commands.size() > 0) {
        // std::cout << "Running commands:" << std::endl;
        // for (std::string command : commands) {
    std::cout << command << std::endl << std::endl;
        // }
        // for (std::string command : commands) {
    std::pair<int, std::string> err = OS::run_command(command);
        //     if (check_error_passive(err.first, err.second)) return 1;
        // }
    std::cout << "Compiled successfully!\n" << std::endl;
    // }
    // else {
    //     std::cout << "No changes detected. Recompilation not necessary." << std::endl;
    // }
    // Handle post-build scripts
    if (check_post_build(L)) run_post_build_script(L);

    // Close lua state
    lua_close(L);

    return 0;
}
