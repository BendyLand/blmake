-- This will be the file where I plan the logic using lua directly (before translating to C++)

require "blmake"

for key, val in pairs(Full_build) do
    print(key .. ": ")
    if type(val) == "table" then
        for k, v in pairs(val) do
            print(k .. ": " .. v)
        end
    else 
        print(val)
    end
    print()
end


--[[ 
Pre-build (Order doesn't matter much):

hooks.pre_build: Execute the pre-build script defined in hooks.pre_build.

Build Steps (Order matters):

Environment Setup:

Set environment variables based on env_vars.
Determine source directory (src_dir) and output directory (out_dir) (optional, might be derived from other settings).
Preprocessing:

Apply preprocessor options (preproc_opts) like macros and include paths.
Apply conditional compilation flags based on build_type using conditional_compilation.
Compilation:

Specify the compiler (compiler).
Compile each source file listed in files.
Include header directories from include_dirs.
Set language standard and extensions with lang_exts.
Apply optimization level (optimization) and debugging flags (debugging).
Apply warnings options from warnings.
Include platform-specific options (platform_opts).
Add custom compiler flags from custom_flags (if applicable).
Linking:

Link compiled object files together.
Use linker options (linker_opts) like library paths and flags (-L, -l).
Link against external libraries listed in dependencies.
Post-build (Order doesn't matter much):

Output: Generate the final executable or library based on the output filename.

Post-build Actions:

Execute post-build commands listed in post_build (e.g., running tests, creating a package).
Logging and Error Handling:

Use logging to specify the log file destination.
Manage errors based on error_handling (e.g., stop on errors, issue warnings).
Additional Notes:

src_dir and out_dir might be used within the build commands to locate source files and place compiled objects.
The specific order within steps (e.g., optimization vs debugging flags) might vary depending on the compiler.
parallel_builds can influence how commands are executed concurrently.
Your build tool should process the full_build config and translate these steps into appropriate commands for your operating system and compiler.
]]

--[[
Starting with the Full_build template and working through its fields, here's a rough order and combination of commands that might be run, assuming every field is filled out. The order follows the typical build process:

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
]]