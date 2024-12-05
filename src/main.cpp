#include "utils.hpp"
#include "build.hpp"
#include "os.hpp"
#include "gen.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    // Create new Lua state for the config file to use
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Load and run the Lua config file
    //! Eventually update this to be ./blmake.lua
    if (luaL_loadfile(L, "src/blmake.lua") != LUA_OK || lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Failed to load config: " << lua_tostring(L, -1) << std::endl;
        return 1;
    }

    // Check for command line args
    if (argc > 1) {
        size_t err = handle_cl_args(argc, argv, L);
        check_error_fatal((int)err, "Error handling command line arguments.");
        return 0;
    }

    // Handle pre-build scripts
    if (check_pre_build(L)) run_pre_build_script(L);

    // Construct and run compilation command
    fs::path path = get_config_value(L, "src_dir") + "/watcher"; 
    if (fs::is_directory(path)) {
        std::vector<std::string> commands = handle_incremental_command_construction(L);
        if (commands.size() > 0) {
            std::cout << "Running commands:" << std::endl;
            for (std::string command : commands) {
                std::cout << command << std::endl << std::endl;
            }
            for (std::string command : commands) {
                std::pair<int, std::string> err = OS::run_command(command);
                if (check_error_passive(err.first, err.second)) return 1;
            }
            std::cout << "Compiled successfully!\n" << std::endl;
        }
        else {
            std::cout << "No changes detected. Recompilation not necessary." << std::endl;
        }
    }
    else {
        std::string command = handle_command_construction(L);
        std::cout << command << std::endl << std::endl;
        std::pair<int, std::string> err = OS::run_command(command);
        std::cout << "Compiled successfully!\n" << std::endl;
    }
    // Handle post-build scripts
    if (check_post_build(L)) run_post_build_script(L);

    // Close lua state
    lua_close(L);

    return 0;
}
