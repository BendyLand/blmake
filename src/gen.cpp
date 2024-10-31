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
    return 1;
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
