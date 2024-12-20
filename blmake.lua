Full_build = {
    compiler = "g++",                           -- The compiler to be used (e.g., "gcc", "clang")
    files = {"main.cpp", "utils.cpp"},          -- A list of source files to compile
    include_dirs = {"include"},                 -- Directories for include files (optional)
    preproc_opts = {"DEBUG"},                   -- Preprocessor options (e.g., macros, include paths)
    linker_opts = {"libs"},                     -- Linker options (i.e. library paths)
    dependencies = {"m", "pthread"},            -- libraries to link against, e.g. {"lib1", "lib2"}
    framework_paths = {},                       -- Linker options (i.e. library paths)
    frameworks = {},                            -- libraries to link against, e.g. {"lib1", "lib2"}
    output = "main",                            -- Output file name (optional, defaults to a.out or similar)
    optimization = "-O2",                       -- Optimization level (e.g., "-O2", "-Os")
    debugging = false,                          -- Debugging options (e.g., "-g")
    warnings = {"all", "error"},                -- Warning levels and options (e.g., "-Wall", "-Werror")
    platform_opts = {"-m64"},                   -- Platform-specific options (e.g., architecture flags)
    profiling = "-pg",                          -- Profiling options (e.g., "-pg")
    lto = "-flto",                              -- Link-time optimization (e.g., "-flto")
    lang_exts = {"-std=c++20"},                 -- Language standard and extensions (e.g., "-std=c11", "-fno-exceptions")
    build_type = "Debug",                       -- or "Release"
    env_vars = {},                              -- e.g., {CC = "gcc", CXX = "g++"}
    src_dir = "",                               -- Source directory
    out_dir = "build",                          -- Output directory
    hooks = {                                   -- e.g., {pre_build = "scripts/pre_build.sh"} 
        pre_build = "scripts/pre_build.sh",
        post_build = "scripts/post_build.sh"
    }, 
    custom_flags = {},                          -- e.g., {"-fno-stack-protector"}
    tests = {},                                 -- e.g., {"test_suite1", "test_suite2"}
    documentation = "",                         -- Directory or command for generating docs
    project_metadata = {                        -- Optional: project name, version, author
        name = "BendyLand",
        version = "0.0.1",
        author = "Ben",
    }
}

-- Customize behavior below!
-- For example:
--[[
if Full_build.build_type == "Debug" then
    Full_build.optimization = "-O0"
end
]]
