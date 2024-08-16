Full_build = {
    compiler = "g++",                  -- The compiler to be used (e.g., "gcc", "clang")
    files = {"main.cpp"},              -- A list of source files to compile
    include_dirs = {""},               -- Directories for include files (optional)
    preproc_opts = {""},               -- Preprocessor options (e.g., macros, include paths)
    linker_opts = {""},                -- Linker options (e.g., libraries, library paths)
    output = "main",                   -- Output file name (optional, defaults to a.out or similar)
    optimization = "",                 -- Optimization level (e.g., "-O2", "-Os")
    debugging = "",                    -- Debugging options (e.g., "-g")
    warnings = {""},                   -- Warning levels and options (e.g., "-Wall", "-Werror")
    platform_opts = {""},              -- Platform-specific options (e.g., architecture flags)
    profiling = "",                    -- Profiling options (e.g., "-pg")
    lto = "",                          -- Link-time optimization (e.g., "-flto")
    lang_exts = {""},                  -- Language standard and extensions (e.g., "-std=c11", "-fno-exceptions")
    targets = {""},                    -- e.g., {"debug", "release"}
    dependencies = {""},               -- e.g., {"lib1", "lib2"}
    post_build = {""},                 -- e.g., {"run_tests", "deploy"}
    build_type = "Debug",              -- or "Release"
    env_vars = {""},                   -- e.g., {CC = "gcc", CXX = "g++"}
    src_dir = "custom_example",        -- Source directory
    out_dir = "custom_example/build",  -- Output directory
    conditional_compilation = {""},    -- e.g., {debug = "-DDEBUG", release = "-DNDEBUG"}
    hooks = {                          -- e.g., {pre_build = "scripts/pre_build.sh"} 
        pre_build = "custom_example/scripts/pre_build.sh"
    }, 
    custom_flags = {""},               -- e.g., {"-fno-stack-protector"}
    tests = {""},                      -- e.g., {"test_suite1", "test_suite2"}
    documentation = "",                -- Directory or command for generating docs
    project_metadata = {               -- Optional: project name, version, author
        name = "",
        version = "",
        author = "",
    },
    parallel_builds = true,            -- or specify number of jobs
    error_handling = "strict",         -- e.g. "strict", "warn"
    logging = ""                       -- Specify log file
}

Simple_build = {
    compiler = "g++",                 -- e.g., "gcc"
    files = {"main.cpp"},             -- e.g., {"main.c", "utils.c"}
    lang_exts = {"-std=c++20"},       -- e.g., {"main.c", "utils.c"}
    include_dirs = {""},              -- e.g., {"include/"}
    output = "main",                  -- e.g., "myprogram"
    debugging = "",                   -- e.g., "-g"
}

--[[
Options Passed Manually by the Programmer

Multiple Source Files
 - Manually compile multiple files or set up a build system:
     - Example: gcc file1.c file2.c -o program
 - Using -c to compile source files into object files:
     - Example: gcc -c file1.c -o file1.o

Preprocessor Options
 - Define macros, include directories:
    - Example: gcc -DNAME=value -I/path/to/include
 
Linker Options
 - Manually specify libraries and library paths:
     - Example: gcc file1.o -L/path/to/libs -lmylib -o program
 - Use of -static for static linking:
     - Example: gcc file1.o -static -o program

Optimization Levels
 - Manually set optimization level:
    - Example: gcc -O2 main.c -o main
    
Debugging Options
 - Enable debug symbols:
     - Example: gcc -g main.c -o main
 - Specify debugging level:
     - Example: gcc -ggdb main.c -o main
     
Warnings and Errors
 - Enable warnings or treat warnings as errors:
     - Example: gcc -Wall -Werror main.c -o main
     
Platform-Specific Options
 - Manually specify target architecture or cross-compilation settings:
     - Example: gcc -march=native -mtune=native main.c -o main
     
Parallel Compilation
 - Manually invoke parallel compilation in make:
     - Example: make -j4
     
Build Systems
 - Manually set up and configure build systems like CMake, Makefile, etc.
 
Profiling and Analysis
 - Enable profiling or sanitizers:
     - Example: gcc -pg main.c -o main
     - Example: gcc -fsanitize=address main.c -o main
     
Link-Time Optimization (LTO)
 - Manually enable LTO:
    - Example: gcc -flto main.c -o main
    
Language Extensions
 - Specify language standards or disable extensions:
     - Example: gcc -std=c11 main.c -o main
     - Example: g++ -std=c++17 -fno-exceptions main.cpp -o main
     
Custom Compiler Flags
 - Manually pass custom flags:
     - Example: gcc -fno-stack-protector main.c -o main
     

Options Handled Automatically by the Compiler (or During Other Steps)

Linker
 - The compiler automatically invokes the linker to produce an executable unless using the -c flag (which only compiles to object files).

Standard Library
 - The compiler automatically links against the C or C++ standard library unless specified otherwise (e.g., with -nostdlib).

Basic Optimization and Warning Defaults
 - Some basic optimizations and warnings might be enabled by default, though more specific levels require manual specification.

Platform-Specific Defaults
 - The compiler automatically detects and optimizes for the host architecture if no specific architecture is provided.

Handling of Precompiled Headers
 - If you use precompiled headers, the compiler handles them automatically after they are set up manually.

Default Include and Library Paths
 - The compiler automatically searches default include directories and library paths unless overridden.

Default Linker and Compiler Settings
 - Defaults for the linker (like searching for libc or standard library paths) are handled automatically unless specific flags override them.

Summary
 - Manually Passed Options: These are mostly related to optimization, debugging, linking, specific architecture settings, custom flags, and more complex build systems.
 - Automatically Handled Options: These include linking, standard library inclusion, basic default settings, and handling of default paths and system architecture.
]]
