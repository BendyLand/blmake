#include "gen.hpp"

int handle_cl_args(int argc, char** argv)
{
    if (argv[1] == std::string("gen")) {
        size_t err = handle_template_generation(argc, argv);
        if (err) return 1;
        return 0;
    }
    else if (argv[1] == std::string("help")) {
        print_help_menu();
        return 0;
    }
}

size_t generate_full_build()
{
    int err = copy_template_file("src/templates/full_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_build()
{
    int err = copy_template_file("src/templates/build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_simple_build()
{
    int err = copy_template_file("src/templates/simple_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_tiny_build()
{
    int err = copy_template_file("src/templates/tiny_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_test_build()
{
    int err = copy_template_file("src/templates/test_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t handle_template_generation(int argc, char** argv)
{
    if (argc > 2) {
        std::string check = to_lower(argv[2]);
        if (check == std::string("full")) {
            std::cout << "Generating Full_build in blmake.lua..." << std::endl;
            size_t err = generate_full_build();
            if (err) exit(1);
        }
        else if (check == std::string("build")) {
            std::cout << "Generating Build in blmake.lua..." << std::endl;
            size_t err = generate_build();
            if (err) exit(1);
        }
        else if (check == std::string("simple")) {
            std::cout << "Generating Simple_build in blmake.lua..." << std::endl;
            size_t err = generate_simple_build();
            if (err) exit(1);
        }
        else if (check == std::string("tiny")) {
            std::cout << "Generating Tiny_build in blmake.lua..." << std::endl;
            size_t err = generate_tiny_build();
            if (err) exit(1);
        }
        else if (check == std::string("test")) {
            std::cout << "Generating Test_build in blmake.lua..." << std::endl;
            size_t err = generate_test_build();
            if (err) exit(1);
        }
        else {
            std::cerr << "Unknown template type." << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "Generating Full_build in blmake.lua..." << std::endl;
        size_t err = generate_full_build();
        if (err) exit(1);
    }
    return 0;
}

// Helper function to get a string field from the Lua table
std::string get_lua_string(lua_State* L, const char* name) 
{
    lua_getfield(L, -1, name);
    std::string result = lua_tostring(L, -1);
    lua_pop(L, 1); // Pop the value off the stack
    return result;
}

// Helper function to get a table of strings from the Lua file
std::vector<std::string> get_lua_string_array(lua_State* L, const char* name) 
{
    std::vector<std::string> result;
    lua_getfield(L, -1, name); // Push table onto the stack

    if (lua_istable(L, -1)) {
        lua_pushnil(L); // Start iterating with the first key
        while (lua_next(L, -2)) {
            result.push_back(lua_tostring(L, -1));
            lua_pop(L, 1); // Pop the value, keep the key for the next iteration
        }
    }
    lua_pop(L, 1); // Pop the table from the stack
    return result;
}

// Main function to generate the CMakeLists.txt
void generate_cmake_from_lua(const std::string& luaFile) 
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    // Load and execute the Lua configuration file
    if (luaL_dofile(L, luaFile.c_str()) != LUA_OK) {
        std::cerr << "Error loading Lua file: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return;
    }
    // Assume the configuration is stored in a global table named "Full_build"
    lua_getglobal(L, "Full_build");
    if (!lua_istable(L, -1)) {
        std::cerr << "Error: Full_build is not a table." << std::endl;
        lua_close(L);
        return;
    }
    // Extract relevant fields from the Lua table
    std::string projectName = get_lua_string(L, "project_metadata.name");
    std::string version = get_lua_string(L, "project_metadata.version");
    std::string compiler = get_lua_string(L, "compiler");
    std::vector<std::string> files = get_lua_string_array(L, "files");
    std::vector<std::string> includeDirs = get_lua_string_array(L, "include_dirs");
    std::vector<std::string> dependencies = get_lua_string_array(L, "dependencies");
    // Open the CMakeLists.txt file for writing
    std::ofstream cmakeFile("CMakeLists.txt");
    if (!cmakeFile.is_open()) {
        std::cerr << "Error: Could not open CMakeLists.txt for writing." << std::endl;
        lua_close(L);
        return;
    }
    // Write CMake content
    cmakeFile << "cmake_minimum_required(VERSION 3.16)\n";
    cmakeFile << "project(" << projectName << " VERSION " << version << ")\n";
    cmakeFile << "set(CMAKE_CXX_COMPILER \"" << compiler << "\")\n";
    // Add source files
    cmakeFile << "add_executable(" << projectName;
    for (const auto& file : files) {
        cmakeFile << " " << file;
    }
    cmakeFile << ")\n";
    // Add include directories
    if (!includeDirs.empty()) {
        cmakeFile << "include_directories(";
        for (const auto& dir : includeDirs) {
            cmakeFile << dir << " ";
        }
        cmakeFile << ")\n";
    }
    // Link libraries
    if (!dependencies.empty()) {
        cmakeFile << "target_link_libraries(" << projectName << " PRIVATE ";
        for (const auto& dep : dependencies) {
            cmakeFile << dep << " ";
        }
        cmakeFile << ")\n";
    }
    // Close the file and Lua state
    cmakeFile.close();
    lua_close(L);
    std::cout << "CMakeLists.txt generated successfully." << std::endl;
}
