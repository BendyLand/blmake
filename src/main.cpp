#include <iostream>
#include "utils.hpp"

std::string construct_full_build_command(lua_State* L);

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Load and run the Lua configuration file
    if (luaL_loadfile(L, "src/blmake.lua") != LUA_OK || lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Failed to load config: " << lua_tostring(L, -1) << std::endl;
        return 1;
    }
    if (check_table(L, "Full_build")) {
        std::string command = construct_full_build_command(L);
        std::cout << command << std::endl;
    }
    else if (check_table(L, "Build")) {
        std::cout << "Reading Build table..." << std::endl;
    }
    else if (check_table(L, "Simple_build")) {
        std::cout << "Reading Simple_build table..." << std::endl;
    }
    else if (check_table(L, "Tiny_build")) {
        std::cout << "Reading Tiny_build table..." << std::endl;
    }
	else {
		std::cout << "Could not find config table." << std::endl;
	}
    lua_close(L);
    return 0;
}

std::string construct_full_build_command(lua_State* L)
{
    std::string command = "";
    lua_getglobal(L, "Full_build");
    lua_getfield(L, -1, "compiler");
    if (lua_isstring(L, -1)) {
        std::string compiler = lua_tostring(L, -1);
        sanitize(compiler);
        if (is_valid_compiler(compiler)) {
            command += compiler;
        }
        else {
            std::cerr << "Error: Invalid compiler." << std::endl;
            print_valid_compilers();
            exit(1);
        }
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "preproc_opts");
    if (lua_istable(L, -1)) {
        std::string preproc_opts = get_table_commands(L, "");
        sanitize(preproc_opts);
        command += " " + preproc_opts;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "lang_exts");
    if (lua_istable(L, -1)) {
        std::string lang_exts = get_table_commands(L, "");
        sanitize(lang_exts);
        command += " " + lang_exts;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "optimization");
    if (lua_isstring(L, -1)) {
        std::string optimization = lua_tostring(L, -1);
        sanitize(optimization);
        if (optimization.size() > 0) {
            command += " " + optimization;
        }
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "profiling");
    if (lua_isstring(L, -1)) {
        std::string profiling = lua_tostring(L, -1);
        sanitize(profiling);
        if (profiling.size() > 0) {
            command += " " + profiling;
        }
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "platform_opts");
    if (lua_istable(L, -1)) {
        std::string platform_opts = get_table_commands(L, "");
        sanitize(platform_opts);
        command += " " + platform_opts;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "include_dirs");
    if (lua_istable(L, -1)) {
        std::string include_dirs = get_table_commands(L, "-I");
        sanitize(include_dirs);
        command += " " + include_dirs;
        lua_pop(L, 1);
    }
    std::string prefix = "";
    lua_getfield(L, -1, "src_dir");
    if (lua_isstring(L, -1)) {
        std::string src_dir = lua_tostring(L, -1);
        sanitize(src_dir);
        prefix = src_dir;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "files");
    if (lua_istable(L, -1)) {
        if (prefix.size() > 0) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        command += " " + files;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "lto");
    if (lua_isstring(L, -1)) {
        std::string lto = lua_tostring(L, -1);
        sanitize(lto);
        if (lto.size() > 0) {
            command += " " + lto;
        }
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "linker_opts");
    if (lua_istable(L, -1)) {
        std::string linker_opts = get_table_commands(L, "-L");
        sanitize(linker_opts);
        command += " " + linker_opts;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "dependencies");
    if (lua_istable(L, -1)) {
        std::string dependencies = get_table_commands(L, "-l");
        sanitize(dependencies);
        command += " " + dependencies;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "warnings");
    if (lua_istable(L, -1)) {
        std::string warnings = get_table_commands(L, "-W");
        sanitize(warnings);
        command += " " + warnings;
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g";
        }
        lua_pop(L, 1);
    }
    std::string output = "";
    lua_getfield(L, -1, "out_dir");
    if (lua_isstring(L, -1)) {
        std::string out_dir = lua_tostring(L, -1);
        sanitize(out_dir);
        if (out_dir.size() > 0) {
            output += out_dir + "/";
        }
        lua_pop(L, 1);
    }
    lua_getfield(L, -1, "output");
    if (lua_isstring(L, -1)) {
        std::string out = lua_tostring(L, -1);
        sanitize(out);
        if (out.size() > 0) {
            output += out; 
        }
        lua_pop(L, 1);
    }
    if (output.size() > 0) {
        command += " -o " + output;
    }
    return command;
}

/*
1. Pre-Build Hooks
Command:
bash custom_example/scripts/pre_build.sh
Purpose: Execute any pre-build tasks specified in the hooks section. This might include cleaning the build directory, setting up environment variables, or generating files.

2. Environment Setup
Commands:
export CC=gcc
export CXX=g++
Purpose: Set up environment variables specified in env_vars. These may influence compiler choices or other toolchain aspects.

3. Conditional Compilation
Command Example:
g++ -DDEBUG main.cpp utils.cpp -o custom_example/build/main_debug
Purpose: Handle conditional_compilation by applying different preprocessor options based on the build_type (e.g., Debug or Release). You might have separate sets of flags for different targets like Debug and Release.

4. Compilation
Command Example:
g++ -Icustom_example/include -std=c++20 -O2 -g -Wall -Werror -fno-stack-protector -c main.cpp -o custom_example/build/main.o
g++ -Icustom_example/include -std=c++20 -O2 -g -Wall -Werror -fno-stack-protector -c utils.cpp -o custom_example/build/utils.o
Purpose: Compile each source file listed in files into object files, considering the include_dirs, preproc_opts, optimization, debugging, warnings, platform_opts, lang_exts, and custom_flags.

5. Linking
Command Example:
g++ custom_example/build/main.o custom_example/build/utils.o -L/some/library/path -lsomelib -o custom_example/build/main -flto -pg
Purpose: Link the object files together into the final binary, considering the linker_opts, output, profiling, lto, dependencies, and any platform-specific options.

6. Parallel Builds
Command Example:
make -j4
Purpose: If parallel_builds is set to true (or a number is specified), execute the build process using multiple jobs to speed up compilation and linking.

7. Post-Build Hooks
Command:
bash custom_example/scripts/run_tests.sh
bash custom_example/scripts/deploy.sh
Purpose: Run any post-build scripts or commands specified in post_build, such as running tests or deploying the binary.

8. Testing
Command Example:
./custom_example/build/test_suite1
./custom_example/build/test_suite2
Purpose: Execute the test suites listed in tests to verify the build. This step might also be part of the post-build process.

9. Documentation Generation
Command:
doxygen custom_example/Doxyfile
Purpose: Generate documentation if the documentation field specifies a command or a directory where the documentation process should be triggered.

10. Logging
Command Example:
make > build.log 2>&1
Purpose: Capture the output of the entire build process in a log file if logging is specified. This is useful for later debugging or review.

11. Error Handling
Command:
set -e  # Abort on any error
Purpose: Depending on the error_handling field (e.g., "strict"), configure the build tool to stop execution on errors or to continue with warnings.

Command Sequence Summary:
1)  Run Pre-Build Hooks
2)  Set Environment Variables
3)  Apply Conditional Compilation
4)  Compile Source Files
5)  Link Object Files
6)  Use Parallel Builds if Enabled
7)  Run Post-Build Hooks
8)  Execute Test Suites
9)  Generate Documentation
10) Log Build Process
11) Handle Errors Strictly or With Warnings
This sequence ensures that every aspect of the Full_build template is covered. The specific commands might vary based on the fields’ values, but this general flow should serve as a solid starting point for implementing the build process.
*/
