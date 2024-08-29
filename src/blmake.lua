Full_build = {
    compiler = "g++",                  -- The compiler to be used (e.g., "gcc", "clang")
    files = {"main.cpp", "utils.cpp"}, -- A list of source files to compile
    include_dirs = {"custom_example/include"}, -- Directories for include files (optional)
    preproc_opts = {"-DDEBUG"},               -- Preprocessor options (e.g., macros, include paths)
    linker_opts = {"custom_example/libs"},                -- Linker options (e.g., libraries, library paths)
    output = "main",                   -- Output file name (optional, defaults to a.out or similar)
    optimization = "-O2",                 -- Optimization level (e.g., "-O2", "-Os")
    debugging = true,                    -- Debugging options (e.g., "-g")
    warnings = {"all", "error"},       -- Warning levels and options (e.g., "-Wall", "-Werror")
    platform_opts = {"-m64"},              -- Platform-specific options (e.g., architecture flags)
    profiling = "-pg",                    -- Profiling options (e.g., "-pg")
    lto = "-flto",                          -- Link-time optimization (e.g., "-flto")
    lang_exts = {"-std=c++20"},        -- Language standard and extensions (e.g., "-std=c11", "-fno-exceptions")
    targets = {"debug"},                    -- e.g., {"debug", "release"}
    dependencies = {"m", "pthread"},               -- e.g., {"lib1", "lib2"}
    post_build = {""},                 -- e.g., {"run_tests", "deploy"}
    build_type = "Debug",              -- or "Release"
    env_vars = {""},                   -- e.g., {CC = "gcc", CXX = "g++"}
    src_dir = "custom_example",        -- Source directory
    out_dir = "custom_example/build",  -- Output directory
    hooks = {                          -- e.g., {pre_build = "scripts/pre_build.sh"} 
        pre_build = "custom_example/scripts/pre_build.sh"
    }, 
    custom_flags = {""},               -- e.g., {"-fno-stack-protector"}
    tests = {""},                      -- e.g., {"test_suite1", "test_suite2"}
    documentation = "",                -- Directory or command for generating docs
    project_metadata = {               -- Optional: project name, version, author
        name = "BendyLand",
        version = "0.0.1",
        author = "Ben",
    },
    error_handling = "strict",         -- e.g. "strict", "warn"
    logging = ""                       -- Specify log file
}

Build = {
    compiler = "g++",                 -- e.g., "gcc"
    files = {"main.cpp", "utils.cpp"},-- e.g., {"main.c", "utils.c"}
    lang_exts = {"-std=c++20"},       -- e.g., {"-std=c++20"}
    include_dirs = {"custom_example/include"}, -- Directories for include files (optional)
    preproc_opts = {""},               -- Preprocessor options (e.g., macros, include paths)
    linker_opts = {""},                -- Linker options (e.g., libraries, library paths)
    optimization = "",                 -- Optimization level (e.g., "-O2", "-Os")
    debugging = "",                    -- Debugging options (e.g., "-g")
    output = "main",                  -- e.g., "myprogram"
    src_dir = "custom_example",        -- Source directory
    out_dir = "custom_example/build",  -- Output directory
}

Test_build = {
    compiler = "g++",                 -- e.g., "gcc"
    files = {"main.cpp", "utils.cpp"},-- e.g., {"main.c", "utils.c"}
    lang_exts = {"-std=c++20"},       -- e.g., {"-std=c++20"}
    include_dirs = {"custom_example/include"}, -- Directories for include files (optional)
    preproc_opts = {""},               -- Preprocessor options (e.g., macros, include paths)
    linker_opts = {""},                -- Linker options (e.g., libraries, library paths)
    optimization = "",                 -- Optimization level (e.g., "-O2", "-Os")
    debugging = "",                    -- Debugging options (e.g., "-g")
    output = "main",                  -- e.g., "myprogram"
    debugging = "",                   -- e.g., "-g"
    src_dir = "custom_example",        -- Source directory
    out_dir = "custom_example/build",  -- Output directory
    tests = {""},                      -- e.g., {"test_suite1", "test_suite2"}
    post_build = {""},                 -- e.g., {"run_tests", "deploy"}
    hooks = {                          -- e.g., {pre_build = "scripts/pre_build.sh"} 
        pre_build = "custom_example/scripts/pre_build.sh"
    }, 
}

Simple_build = {
    compiler = "g++",                 -- e.g., "gcc"
    files = {"main.cpp", "utils.cpp"},-- e.g., {"main.c", "utils.c"}
    lang_exts = {"-std=c++20"},       -- e.g., {"-std=c++20"}
    include_dirs = {""},              -- e.g., {"include/"}
    output = "main",                  -- e.g., "myprogram"
    debugging = "",                   -- e.g., "-g"
}

Tiny_build = {
    compiler = "g++",
    files = {"main.cpp", "utils.cpp"}, 
    output = "main" 
}

Test_str = "This is a test string variable"
