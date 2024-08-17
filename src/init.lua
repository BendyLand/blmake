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
