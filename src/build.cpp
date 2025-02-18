#include "build.hpp"

#if OS_UNIX_LIKE_DEFINED
namespace fs = std::filesystem;
#endif

void run_pre_build_script(lua_State* L)
{
    std::string pre_build_path = extract_pre_build_path(L);
    if (validate_script_ext(pre_build_path)) {
        if (ensure_executable(pre_build_path.c_str())) {
            std::cout << "Running pre-build script: " << pre_build_path << std::endl;
            std::pair<int, std::string> err = OS::run_command(pre_build_path);
            std::string err_message = "Error running pre-build script: " + pre_build_path; 
            check_error_fatal(err.first, err_message);
            if (err.second.size() > 0) {
                std::cout << err.second << std::endl;
            }
        }
    }
    else {
        std::cout << "Error: Invalid extension. No script run." << std::endl;
    }
}

void run_post_build_script(lua_State* L)
{
    std::string post_build_path = extract_post_build_path(L);
    if (validate_script_ext(post_build_path)) {
        if (ensure_executable(post_build_path.c_str())) {
            std::cout << "Running post-build script: " << post_build_path.c_str() << std::endl;
            std::pair<int, std::string> err = OS::run_command(post_build_path);
            std::string message = "Error running post-build script: " + post_build_path; 
            check_error_fatal(err.first, message);
            if (err.second.size() > 0) {
                std::cout << err.second << std::endl;
            }
        }
    }
    else {
        std::cout << "Error: Invalid extension. No script run." << std::endl;
    }
}

std::vector<std::string> construct_incremental_full_build_commands(lua_State* L)
{
    std::string command = "";
    std::vector<std::string> result;
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
    std::string temp_cmd = get_lua_table_with_cmds_as_str(L, "preproc_opts", "-D");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string optimization = get_lua_str(L, "optimization");
    if (!optimization.empty()) command += " " + optimization;
    std::string profiling = get_lua_str(L, "profiling");
    if (!profiling.empty()) command += " " + profiling;
    std::string platform_opts = get_lua_table_as_str(L, "platform_opts");
    if (!platform_opts.empty()) command += " " + platform_opts;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    temp_cmd = get_lua_str(L, "lto");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "warnings", "-W");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "framework_paths", "-F");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "frameworks", "-framework ");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
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
    command += " -c ";
    lua_getfield(L, -1, "files");
    std::string watcher_cmd = get_lua_str(L, "src_dir");
    if (!watcher_cmd.empty()) watcher_cmd += "/";
    watcher_cmd += "watcher/watcher " + prefix;
    if (watcher_cmd.ends_with(" ")) watcher_cmd += ".";
    OS::run_command(watcher_cmd);
    fs::path check_file = fs::path(read_file(fs::path(get_lua_str(L, "src/dir")) / "watcher/recompile_list.txt"));
    std::vector<std::string> file_list; 
    std::vector<std::string> all_files; 
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        all_files = split(files, " ");
        files = filter_files(files, check_file.string());
        file_list = filter_file_list(split(files, " "));
        if (file_list.size() == 0) return result;
        for (std::string file : file_list) {
            std::string temp = command + file + " ";
            std::string output = "-o " + file.substr(0, file.find_last_of(".")) + ".o";
            result.emplace_back(temp + output);
        }
    }
    lua_pop(L, 1);
    std::string link_command = "";
    lua_getfield(L, -1, "compiler");
    if (lua_isstring(L, -1)) {
        std::string compiler = lua_tostring(L, -1);
        sanitize(compiler);
        if (is_valid_compiler(compiler)) {
            link_command += compiler;
        }
        else {
            std::cerr << "Error: Invalid compiler." << std::endl;
            print_valid_compilers();
            exit(1);
        }
    }
    lua_pop(L, 1);
    if (!optimization.empty()) link_command += " " + optimization;
    if (!profiling.empty()) link_command += " " + profiling;
    if (!platform_opts.empty()) link_command += " " + platform_opts;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "framework_paths", "-F");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "frameworks", "-framework ");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    for (std::string file : all_files) {
        std::string output = file.substr(0, file.find_last_of(".")) + ".o";
        link_command += " " + output;
    }
    std::string out_dir = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) out_dir += "-o " + temp_cmd + "/";
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) {
        if (!out_dir.empty()) out_dir += temp_cmd; 
        else out_dir += "-o " + temp_cmd;
    }
    link_command += " " + out_dir;
    result.emplace_back(link_command);
    return result;
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
    std::string temp_cmd = get_lua_table_with_cmds_as_str(L, "preproc_opts", "-D");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_str(L, "optimization");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_str(L, "profiling");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "platform_opts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    if (!prefix.empty()) prefix += "/";
    temp_cmd = get_lua_table_with_cmds_as_str(L, "files", prefix);
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_str(L, "lto");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "framework_paths", "-F");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "frameworks", "-framework ");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "warnings", "-W");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    // debugging expects a boolean
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) output += temp_cmd + "/";
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) output += temp_cmd;
    if (!output.empty()) command += " -o " + output;
    return command;
}

std::vector<std::string> construct_incremental_build_commands(lua_State* L)
{
    std::string command = "";
    std::vector<std::string> result;
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
    std::string temp_cmd = get_lua_table_with_cmds_as_str(L, "preproc_opts", "-D");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string optimization = get_lua_str(L, "optimization");
    if (!optimization.empty()) command += " " + optimization;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);

    std::string output = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) output += temp_cmd + "/";
    command += " -c ";
    lua_getfield(L, -1, "files");
    std::string watcher_cmd = "src/watcher/watcher " + prefix;
    if (watcher_cmd.ends_with(" ")) watcher_cmd += ".";
    OS::run_command(watcher_cmd);
    std::string check_file = read_file("src/watcher/recompile_list.txt");
    std::vector<std::string> file_list; 
    std::vector<std::string> all_files; 
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        all_files = split(files, " ");
        files = filter_files(files, check_file);
        file_list = filter_file_list(split(files, " "));
        if (file_list.size() == 0) return result;
        for (std::string file : file_list) {
            std::string temp = command + file + " ";
            std::string output = "-o " + file.substr(0, file.find_last_of(".")) + ".o";
            result.emplace_back(temp + output);
        }
    }
    lua_pop(L, 1);
    std::string link_command = "";
    lua_getfield(L, -1, "compiler");
    if (lua_isstring(L, -1)) {
        std::string compiler = lua_tostring(L, -1);
        sanitize(compiler);
        if (is_valid_compiler(compiler)) {
            link_command += compiler;
        }
        else {
            std::cerr << "Error: Invalid compiler." << std::endl;
            print_valid_compilers();
            exit(1);
        }
    }
    lua_pop(L, 1);
    if (!optimization.empty()) link_command += " " + optimization;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    for (std::string file : all_files) {
        std::string output = file.substr(0, file.find_last_of(".")) + ".o";
        link_command += " " + output;
    }
    std::string out_dir = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) out_dir += "-o " + temp_cmd + "/";
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) {
        if (!out_dir.empty()) out_dir += temp_cmd; 
        else out_dir += "-o " + temp_cmd;
    }
    link_command += " " + out_dir;
    result.emplace_back(link_command);
    return result;
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
    std::string temp_cmd = get_lua_table_with_cmds_as_str(L, "preproc_opts", "-D");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_str(L, "optimization");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    if (!prefix.empty()) prefix += "/";
    temp_cmd = get_lua_table_with_cmds_as_str(L, "files", prefix);
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) output += temp_cmd + "/";
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) output += temp_cmd;
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}


std::vector<std::string> construct_incremental_simple_build_commands(lua_State* L)
{
    std::string command = "";
    std::vector<std::string> result;
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
    std::string temp_cmd = get_lua_table_as_str(L, "preproc_opts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string optimization = get_lua_str(L, "optimization");
    if (!optimization.empty()) command += " " + optimization;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) output += temp_cmd + "/";
    command += " -c ";
    lua_getfield(L, -1, "files");
    std::string watcher_cmd = "src/watcher/watcher " + prefix;
    if (watcher_cmd.ends_with(" ")) watcher_cmd += ".";
    OS::run_command(watcher_cmd);
    std::string check_file = read_file("src/watcher/recompile_list.txt");
    std::vector<std::string> file_list; 
    std::vector<std::string> all_files; 
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        all_files = split(files, " ");
        files = filter_files(files, check_file);
        file_list = filter_file_list(split(files, " "));
        if (file_list.size() == 0) return result;
        for (std::string file : file_list) {
            std::string temp = command + file + " ";
            std::string output = "-o " + file.substr(0, file.find_last_of(".")) + ".o";
            result.emplace_back(temp + output);
        }
    }
    lua_pop(L, 1);
    std::string link_command = "";
    lua_getfield(L, -1, "compiler");
    if (lua_isstring(L, -1)) {
        std::string compiler = lua_tostring(L, -1);
        sanitize(compiler);
        if (is_valid_compiler(compiler)) {
            link_command += compiler;
        }
        else {
            std::cerr << "Error: Invalid compiler." << std::endl;
            print_valid_compilers();
            exit(1);
        }
    }
    lua_pop(L, 1);
    if (!optimization.empty()) link_command += " " + optimization;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    for (std::string file : all_files) {
        std::string output = file.substr(0, file.find_last_of(".")) + ".o";
        link_command += " " + output;
    }
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) link_command += " -o " + temp_cmd;
    result.emplace_back(link_command);
    return result;
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
    std::string temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "files");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);
    std::string output = get_lua_str(L, "output");
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::vector<std::string> construct_incremental_tiny_build_commands(lua_State* L)
{
    std::string command = "";
    std::vector<std::string> result;
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
    }
    lua_pop(L, 1);
    command += " -c ";
    lua_getfield(L, -1, "files");
    std::string watcher_cmd = "src/watcher/watcher ";
    if (watcher_cmd.ends_with(" ")) watcher_cmd += ".";
    OS::run_command(watcher_cmd);
    std::string check_file = read_file("src/watcher/recompile_list.txt");
    std::vector<std::string> file_list; 
    std::vector<std::string> all_files; 
    if (lua_istable(L, -1)) {
        std::string files = get_table_commands(L, "");
        sanitize(files);
        all_files = split(files, " ");
        files = filter_files(files, check_file);
        file_list = filter_file_list(split(files, " "));
        if (file_list.size() == 0) return result;
        for (std::string file : file_list) {
            std::string temp = command + file + " ";
            std::string output = "-o " + file.substr(0, file.find_last_of(".")) + ".o";
            result.emplace_back(temp + output);
        }
    }
    lua_pop(L, 1);
    std::string link_command = "";
    lua_getfield(L, -1, "compiler");
    if (lua_isstring(L, -1)) {
        std::string compiler = lua_tostring(L, -1);
        sanitize(compiler);
        if (is_valid_compiler(compiler)) {
            link_command += compiler;
        }
        else {
            std::cerr << "Error: Invalid compiler." << std::endl;
            print_valid_compilers();
            exit(1);
        }
    }
    lua_pop(L, 1);
    for (std::string file : all_files) {
        std::string output = file.substr(0, file.find_last_of(".")) + ".o";
        link_command += " " + output;
    }
    std::string temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) link_command += " -o " + temp_cmd;
    result.emplace_back(link_command);
    return result;
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
    std::string files = get_lua_table_as_str(L, "files");
    if (!files.empty()) command += " " + files;
    std::string output = get_lua_str(L, "output");
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::vector<std::string> construct_incremental_test_build_commands(lua_State* L)
{
    std::string command = "";
    std::vector<std::string> result;
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
    std::string temp_cmd = get_lua_table_with_cmds_as_str(L, "preproc_opts", "-D");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string optimization = get_lua_str(L, "optimization");
    if (!optimization.empty()) command += " " + optimization;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "warnings", "-W");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    if (!prefix.empty()) prefix += "/";
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) output += temp_cmd + "/";
    command += " -c ";
    lua_getfield(L, -1, "files");
    std::string watcher_cmd = "src/watcher/watcher " + prefix;
    if (watcher_cmd.ends_with(" ")) watcher_cmd += ".";
    OS::run_command(watcher_cmd);
    std::string check_file = read_file("src/watcher/recompile_list.txt");
    std::vector<std::string> file_list; 
    std::vector<std::string> all_files; 
    if (lua_istable(L, -1)) {
        if (!prefix.empty()) prefix += "/";
        std::string files = get_table_commands(L, prefix);
        sanitize(files);
        all_files = split(files, " ");
        files = filter_files(files, check_file);
        file_list = filter_file_list(split(files, " "));
        if (file_list.size() == 0) return result;
        for (std::string file : file_list) {
            std::string temp = command + file + " ";
            std::string output = "-o " + file.substr(0, file.find_last_of(".")) + ".o";
            result.emplace_back(temp + output);
        }
    }
    lua_pop(L, 1);
    std::string link_command = "";
    lua_getfield(L, -1, "compiler");
    if (lua_isstring(L, -1)) {
        std::string compiler = lua_tostring(L, -1);
        sanitize(compiler);
        if (is_valid_compiler(compiler)) {
            link_command += compiler;
        }
        else {
            std::cerr << "Error: Invalid compiler." << std::endl;
            print_valid_compilers();
            exit(1);
        }
    }
    lua_pop(L, 1);
    if (!optimization.empty()) link_command += " " + optimization;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) link_command += " " + temp_cmd;
    for (std::string file : all_files) {
        std::string output = file.substr(0, file.find_last_of(".")) + ".o";
        link_command += " " + output;
    }
    std::string out_dir = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) out_dir += "-o " + temp_cmd + "/";
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) out_dir += temp_cmd;
    link_command += " " + out_dir;
    result.emplace_back(link_command);
    return result;
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
    std::string temp_cmd = get_lua_table_with_cmds_as_str(L, "preproc_opts", "-D");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_as_str(L, "lang_exts");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_str(L, "optimization");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "include_dirs", "-I");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    std::string prefix = get_lua_str(L, "src_dir");
    if (!prefix.empty()) prefix += "/";
    temp_cmd = get_lua_table_with_cmds_as_str(L, "files", prefix);
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "linker_opts", "-L");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "dependencies", "-l");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    temp_cmd = get_lua_table_with_cmds_as_str(L, "warnings", "-W");
    if (!temp_cmd.empty()) command += " " + temp_cmd;
    lua_getfield(L, -1, "debugging");
    if (lua_isboolean(L, -1)) {
        bool debugging = lua_toboolean(L, -1);
        if (debugging) {
            command += " -g ";
        }
    }
    lua_pop(L, 1);
    std::string output = "";
    temp_cmd = get_lua_str(L, "out_dir");
    if (!temp_cmd.empty()) output += temp_cmd + "/";
    temp_cmd = get_lua_str(L, "output");
    if (!temp_cmd.empty()) output += temp_cmd;
    if (!output.empty()) {
        command += " -o " + output;
    }
    return command;
}

std::string handle_command_construction(lua_State* L)
{
    std::string command; 
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
        exit(1);
	}
    return command;
}

std::vector<std::string> handle_incremental_command_construction(lua_State* L)
{
    std::vector<std::string> commands;
    if (check_table(L, "Full_build")) {
        commands = construct_incremental_full_build_commands(L);
    }
    else if (check_table(L, "Build")) {
        commands = construct_incremental_build_commands(L);
    }
    else if (check_table(L, "Simple_build")) {
        commands = construct_incremental_simple_build_commands(L);
    }
    else if (check_table(L, "Tiny_build")) {
        commands = construct_incremental_tiny_build_commands(L);
    }
    else if (check_table(L, "Test_build")) {
        commands = construct_incremental_test_build_commands(L);
    }
	  else {
        std::cerr << "No valid config tables found." << std::endl;
        exit(1);
	  }
    if (commands.size() == 0) return {};
    return commands;
}

std::string get_blmake_config_type(lua_State* L)
{
    std::string result;
    if (check_table(L, "Full_build")) {
        result = "Full_build";
    }
    else if (check_table(L, "Build")) {
        result = "Build";
    }
    else if (check_table(L, "Simple_build")) {
        result = "Simple_build";
    }
    else if (check_table(L, "Tiny_build")) {
        result = "Tiny_build";
    }
    else if (check_table(L, "Test_build")) {
        result = "Test_build";
    }
	else {
        std::cerr << "No valid config tables found." << std::endl;
        exit(1);
	}
    return result;
}

std::string get_config_value(lua_State* L, std::string field)
{
	std::string result = "";
    std::string config_type = get_blmake_config_type(L);
    lua_getglobal(L, config_type.c_str());
    lua_getfield(L, -1, field.c_str());
    if (lua_isstring(L, -1)) {
        std::string value = lua_tostring(L, -1);
        sanitize(value);
        if (!value.empty()) {
            result = value; 
        }
    }
    else if (lua_istable(L, -1)) {
        result = get_table_commands(L, "");
    }
    return result;
}

void clean_prev_json(lua_State* L)
{
    std::string path = get_config_value(L, "src_dir");
    if (path.size() > 0) path += "/watcher/prev.json";
    else path = "watcher/prev.json";
    write_string_to_file(path, "{}");
    std::cout << "prev.json cleaned successfully!" << std::endl;
}
