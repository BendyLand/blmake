#include "build.hpp"

void run_pre_build_script(lua_State* L)
{
    std::string pre_build_path = extract_pre_build_path(L);
    if (validate_script_ext(pre_build_path)) {
        if (ensure_executable(pre_build_path.c_str())) {
            std::cout << "Running pre-build script: " << pre_build_path << std::endl;
            int err = OS::run_command(pre_build_path);
            if (err) {
                std::cerr << "Error running pre-build script: " << pre_build_path << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        std::cout << "Error: Invalid extension. No script run." << std::endl;
    }
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
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "preproc_opts");
    if (lua_istable(L, -1)) {
        std::string preproc_opts = get_table_commands(L, "");
        sanitize(preproc_opts);
        if (!preproc_opts.empty()) {
            command += " " + preproc_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "lang_exts");
    if (lua_istable(L, -1)) {
        std::string lang_exts = get_table_commands(L, "");
        sanitize(lang_exts);
        if (!lang_exts.empty()) {
            command += " " + lang_exts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "optimization");
    if (lua_isstring(L, -1)) {
        std::string optimization = lua_tostring(L, -1);
        sanitize(optimization);
        if (!optimization.empty()) {
            command += " " + optimization;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "profiling");
    if (lua_isstring(L, -1)) {
        std::string profiling = lua_tostring(L, -1);
        sanitize(profiling);
        if (!profiling.empty()) {
            command += " " + profiling;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "platform_opts");
    if (lua_istable(L, -1)) {
        std::string platform_opts = get_table_commands(L, "");
        sanitize(platform_opts);
        if (!platform_opts.empty()) {
            command += " " + platform_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "include_dirs");
    if (lua_istable(L, -1)) {
        std::string include_dirs = get_table_commands(L, "-I");
        sanitize(include_dirs);
        if (!include_dirs.empty()) {
            command += " " + include_dirs;
        }
    }
    lua_pop(L, 1);
    std::string prefix = "";
    lua_getfield(L, -1, "src_dir");
    if (lua_isstring(L, -1)) {
        std::string src_dir = lua_tostring(L, -1);
        sanitize(src_dir);
        prefix = src_dir;
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "files");
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        if (!files.empty()) {
            command += " " + files;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "lto");
    if (lua_isstring(L, -1)) {
        std::string lto = lua_tostring(L, -1);
        sanitize(lto);
        if (!lto.empty()) {
            command += " " + lto;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "linker_opts");
    if (lua_istable(L, -1)) {
        std::string linker_opts = get_table_commands(L, "-L");
        sanitize(linker_opts);
        if (!linker_opts.empty()) {
            command += " " + linker_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "dependencies");
    if (lua_istable(L, -1)) {
        std::string dependencies = get_table_commands(L, "-l");
        sanitize(dependencies);
        if (!dependencies.empty()) {
            command += " " + dependencies;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "warnings");
    if (lua_istable(L, -1)) {
        std::string warnings = get_table_commands(L, "-W");
        sanitize(warnings);
        if (!warnings.empty()) {
            command += " " + warnings;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    lua_getfield(L, -1, "out_dir");
    if (lua_isstring(L, -1)) {
        std::string out_dir = lua_tostring(L, -1);
        sanitize(out_dir);
        if (!out_dir.empty()) {
            output += out_dir + "/";
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "output");
    if (lua_isstring(L, -1)) {
        std::string out = lua_tostring(L, -1);
        sanitize(out);
        if (!out.empty()) {
            output += out;
        }
    }
    lua_pop(L, 1);
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::string construct_build_command(lua_State* L)
{
    std::string command = "";
    lua_getglobal(L, "Build");
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
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "preproc_opts");
    if (lua_istable(L, -1)) {
        std::string preproc_opts = get_table_commands(L, "");
        sanitize(preproc_opts);
        if (!preproc_opts.empty()) {
            command += " " + preproc_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "lang_exts");
    if (lua_istable(L, -1)) {
        std::string lang_exts = get_table_commands(L, "");
        sanitize(lang_exts);
        if (!lang_exts.empty()) {
            command += " " + lang_exts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "optimization");
    if (lua_isstring(L, -1)) {
        std::string optimization = lua_tostring(L, -1);
        sanitize(optimization);
        if (!optimization.empty()) {
            command += " " + optimization;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "include_dirs");
    if (lua_istable(L, -1)) {
        std::string include_dirs = get_table_commands(L, "-I");
        sanitize(include_dirs);
        if (!include_dirs.empty()) {
            command += " " + include_dirs;
        }
    }
    lua_pop(L, 1);
    std::string prefix = "";
    lua_getfield(L, -1, "src_dir");
    if (lua_isstring(L, -1)) {
        std::string src_dir = lua_tostring(L, -1);
        sanitize(src_dir);
        prefix = src_dir;
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "files");
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        if (!files.empty()) {
            command += " " + files;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "linker_opts");
    if (lua_istable(L, -1)) {
        std::string linker_opts = get_table_commands(L, "-L");
        sanitize(linker_opts);
        if (!linker_opts.empty()) {
            command += " " + linker_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "dependencies");
    if (lua_istable(L, -1)) {
        std::string dependencies = get_table_commands(L, "-l");
        sanitize(dependencies);
        if (!dependencies.empty()) {
            command += " " + dependencies;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    lua_getfield(L, -1, "out_dir");
    if (lua_isstring(L, -1)) {
        std::string out_dir = lua_tostring(L, -1);
        sanitize(out_dir);
        if (!out_dir.empty()) {
            output += out_dir + "/";
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "output");
    if (lua_isstring(L, -1)) {
        std::string out = lua_tostring(L, -1);
        sanitize(out);
        if (!out.empty()) {
            output += out;
        }
    }
    lua_pop(L, 1);
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::string construct_simple_build_command(lua_State* L)
{
    std::string command = "";
    lua_getglobal(L, "Simple_build");
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
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "lang_exts");
    if (lua_istable(L, -1)) {
        std::string lang_exts = get_table_commands(L, "");
        sanitize(lang_exts);
        if (!lang_exts.empty()) {
            command += " " + lang_exts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "include_dirs");
    if (lua_istable(L, -1)) {
        std::string include_dirs = get_table_commands(L, "-I");
        sanitize(include_dirs);
        if (!include_dirs.empty()) {
            command += " " + include_dirs;
        }
    }
    lua_pop(L, 1);
    std::string prefix = "";
    lua_getfield(L, -1, "files");
    if (lua_istable(L, -1)) {
        std::string files = get_table_commands(L, "");
        sanitize(files);
        if (!files.empty()) {
            command += " " + files;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "dependencies");
    if (lua_istable(L, -1)) {
        std::string dependencies = get_table_commands(L, "-l");
        sanitize(dependencies);
        if (!dependencies.empty()) {
            command += " " + dependencies;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g";
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "output");
    std::string output;
    if (lua_isstring(L, -1)) {
        output = lua_tostring(L, -1);
        sanitize(output);
    }
    lua_pop(L, 1);
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::string construct_tiny_build_command(lua_State* L)
{
    std::string command = "";
    lua_getglobal(L, "Tiny_build");
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
    lua_getfield(L, -1, "files");
    if (lua_istable(L, -1)) {
        std::string files = get_table_commands(L, "");
        sanitize(files);
        if (!files.empty()) {
            command += " " + files;
        }
        lua_pop(L, 1);
    }
    std::string output;
    lua_getfield(L, -1, "output");
    if (lua_isstring(L, -1)) {
        output = lua_tostring(L, -1);
        sanitize(output);
        lua_pop(L, 1);
    }
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::string construct_test_build_command(lua_State* L)
{
    std::string command = "";
    lua_getglobal(L, "Test_build");
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
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "preproc_opts");
    if (lua_istable(L, -1)) {
        std::string preproc_opts = get_table_commands(L, "");
        sanitize(preproc_opts);
        if (!preproc_opts.empty()) {
            command += " " + preproc_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "lang_exts");
    if (lua_istable(L, -1)) {
        std::string lang_exts = get_table_commands(L, "");
        sanitize(lang_exts);
        if (!lang_exts.empty()) {
            command += " " + lang_exts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "optimization");
    if (lua_isstring(L, -1)) {
        std::string optimization = lua_tostring(L, -1);
        sanitize(optimization);
        if (!optimization.empty()) {
            command += " " + optimization;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "include_dirs");
    if (lua_istable(L, -1)) {
        std::string include_dirs = get_table_commands(L, "-I");
        sanitize(include_dirs);
        if (!include_dirs.empty()) {
            command += " " + include_dirs;
        }
    }
    lua_pop(L, 1);
    std::string prefix = "";
    lua_getfield(L, -1, "src_dir");
    if (lua_isstring(L, -1)) {
        std::string src_dir = lua_tostring(L, -1);
        sanitize(src_dir);
        prefix = src_dir;
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "files");
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        if (!files.empty()) {
            command += " " + files;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "linker_opts");
    if (lua_istable(L, -1)) {
        std::string linker_opts = get_table_commands(L, "-L");
        sanitize(linker_opts);
        if (!linker_opts.empty()) {
            command += " " + linker_opts;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "dependencies");
    if (lua_istable(L, -1)) {
        std::string dependencies = get_table_commands(L, "-l");
        sanitize(dependencies);
        if (!dependencies.empty()) {
            command += " " + dependencies;
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    lua_getfield(L, -1, "out_dir");
    if (lua_isstring(L, -1)) {
        std::string out_dir = lua_tostring(L, -1);
        sanitize(out_dir);
        if (!out_dir.empty()) {
            output += out_dir + "/";
        }
    }
    lua_pop(L, 1);
    lua_getfield(L, -1, "output");
    if (lua_isstring(L, -1)) {
        std::string out = lua_tostring(L, -1);
        sanitize(out);
        if (!out.empty()) {
            output += out;
        }
    }
    lua_pop(L, 1);
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::string handle_command_construction(lua_State* L)
{
    std::string command = "";
    if (check_table(L, "Full_build")) {
        command = construct_full_build_command(L);
    }
    else if (check_table(L, "Build")) {
        command = construct_build_command(L);
    }
    else if (check_table(L, "Simple_build")) {
        command = construct_simple_build_command(L);
    }
    else if (check_table(L, "Tiny_build")) {
        command = construct_tiny_build_command(L);
    }
    else if (check_table(L, "Test_build")) {
        command = construct_test_build_command(L);
    }
	else {
        std::cerr << "No valid config tables found." << std::endl;
	}
    return command;
}