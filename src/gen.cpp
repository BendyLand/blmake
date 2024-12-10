#include "gen.hpp"
#include "templates/blmake/full_build_template.h"
#include "templates/blmake/build_template.h"
#include "templates/blmake/simple_build_template.h"
#include "templates/blmake/tiny_build_template.h"
#include "templates/blmake/test_build_template.h"
#include "templates/premake/full_build_premake.h"
#include "templates/premake/build_premake.h"
#include "templates/premake/simple_build_premake.h"
#include "templates/premake/tiny_build_premake.h"
#include "templates/premake/test_build_premake.h"
#include "watcher_config.hpp"

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

size_t check_for_gen_arg(int argc, char** argv)
{
    if (argc > 1) {
        if (argv[1] == std::string("gen")) {
            size_t err = Blmake::handle_template_generation(argc, argv);
            return err;
        }
    }
    return 2;
}

size_t handle_cl_args(int argc, char** argv, lua_State* L)
{
    if (argv[1] == std::string("help")) {
        print_help_menu();
        return 0;
    }
    else if (argv[1] == std::string("align")) {
        //todo: write function below
        // align_config_comments();
        return 0;
    }
    else if (argv[1] == std::string("watch")) {
        if (argc > 2) {
            if (argv[2] == std::string("clean")) {
                clean_prev_json(L);
                return 0;
            }
        }
        size_t err = Watcher::generate_watcher_structure(L);
        return err;
    }
    else if (std::string(argv[1]).find("premake") != std::string::npos || std::string(argv[1]) == ("gen")) {
        size_t err = Premake::handle_template_generation(argc, argv, L);
        return err;
    }
    return 1;
}

namespace Premake
{
    std::string generate_premake_build(lua_State* L)
    {
        std::string contents = get_premake_contents(L);
        contents = populate_premake_template(L, contents);
        return contents;
    }

    std::string get_premake_contents(lua_State* L)
    {
        std::string kind = get_blmake_config_type(L);
        std::string result = "";
        if (kind == "Full_build") {
            for (size_t i = 0; i < full_build_premake_txt_len; i++) result += full_build_premake_txt[i];
        }
        else if (kind == "Build") {
            for (size_t i = 0; i < build_premake_txt_len; i++) result += build_premake_txt[i];
        }
        else if (kind == "Simple_build") {
            for (size_t i = 0; i < simple_build_premake_txt_len; i++) result += simple_build_premake_txt[i];
        }
        else if (kind == "Tiny_build") {
            for (size_t i = 0; i < tiny_build_premake_txt_len; i++) result += tiny_build_premake_txt[i];
        }
        else if (kind == "Test_build") {
            for (size_t i = 0; i < test_build_premake_txt_len; i++) result += test_build_premake_txt[i];
        }
        else {
            for (size_t i = 0; i < full_build_premake_txt_len; i++) result += full_build_premake_txt[i];
        }
        return result;
    }

    size_t handle_template_generation(int argc, char** argv, lua_State* L)
    {
        std::string contents = generate_premake_build(L);
        contents = populate_premake_template(L, contents);
        size_t err = write_string_to_file("./premake5.lua", contents);
        if (err == 0) std::cout << "Premake file generated successfully!" << std::endl;
        return err;
    }

    void insert_field_str(std::vector<std::string>*& vec, const std::string& field, std::string& value)
    {
        size_t idx = find(*vec, field);
        size_t dq_idx = (*vec)[idx].find("\"");
        if ((*vec)[idx][dq_idx+1] != '"') return;
        std::string new_line = (*vec)[idx].insert(dq_idx+1, value);
        (*vec)[idx] = new_line;
    }

    void insert_field_tbl(std::vector<std::string>*& vec, const std::string& field, std::string& value)
    {
        size_t idx = find(*vec, field);
        size_t b_idx = (*vec)[idx].find("{}");
        if (b_idx == std::string::npos) return;
        value = add_quotes_and_commas(value);
        std::string new_line = (*vec)[idx].insert(b_idx+1, value + " ");
        b_idx = (*vec)[idx].find("{");
        new_line = new_line.insert(b_idx+1, " ");
        (*vec)[idx] = new_line;
    }

    std::string populate_full_build_premake_template(lua_State* L, const std::string& contents)
    {
        std::string result = contents;
        std::vector<std::string> temp_vec = split(result, "\n");
        std::vector<std::string>* lines = &temp_vec;
        std::string tbl = get_lua_table_as_str(L, "lang_exts");
        std::string dialect = tbl.find("-std=c++") != std::string::npos ? to_uppercase(tbl.substr(tbl.find("-std=c++")+5, 10)) : "";
        std::string language = get_lua_str(L, "compiler");
        if (language == "g++") language = "C++";
        else if (language == "gcc" || language == "clang") language = "C";
        std::unordered_map<std::string, std::string> str_keywords = {
            {"workspace", get_blmake_config_type(L)} ,
            {"location", get_lua_str(L, "out_dir")},
            {"project", get_blmake_config_type(L)},
            {"language", language},
            {"targetdir", get_lua_str(L, "out_dir")},
            {"objdir", get_lua_str(L, "out_dir")+"/obj"},
            {"cppdialect", dialect},
        };
        std::string prefix = get_lua_str(L, "src_dir");
        std::string options = {
            get_lua_table_with_cmds_as_str(L, "warnings", "-W") + " " + 
            get_lua_str(L, "lto") + " " + 
            get_lua_str(L, "profiling")
        };
        options = remove_extra_spaces(options);
        if (!prefix.empty()) prefix += "/";
        std::string prebuild = "";
        std::string postbuild = "";
        lua_getfield(L, -1, "hooks");
        if (lua_istable(L, -1)) {
            lua_getfield(L, -1, "pre_build");
            if (lua_isstring(L, -1)) {
                prebuild = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
            lua_getfield(L, -1, "post_build");
            if (lua_isstring(L, -1)) {
                postbuild = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
        std::unordered_map<std::string, std::string> tbl_keywords = {
            {"files", get_lua_table_with_cmds_as_str(L, "files", prefix)} ,
            {"includedirs", get_lua_table_as_str(L, "include_dirs")},
            {"defines", get_lua_table_as_str(L, "preproc_opts")},
            {"buildoptions", options},
            {"libdirs", get_lua_table_as_str(L, "linker_opts")},
            {"links", get_lua_table_as_str(L, "dependencies")}, 
            {"prebuildcommands", prebuild}, 
            {"postbuildcommands", postbuild}, 
        };
        for (auto& [k, v] : str_keywords) {
            insert_field_str(lines, k, v);
        }
        for (auto& [k, v] : tbl_keywords) {
            insert_field_tbl(lines, k, v);
        }
        result = join(*lines, "\n");
        return result;
    }

    std::string populate_build_premake_template(lua_State* L, const std::string& contents)
    {
        std::string result = contents;
        std::vector<std::string> temp_vec = split(result, "\n");
        std::vector<std::string>* lines = &temp_vec;
        std::string tbl = get_lua_table_as_str(L, "lang_exts");
        std::string dialect = tbl.find("-std=c++") != std::string::npos ? to_uppercase(tbl.substr(tbl.find("-std=c++")+5, 10)) : "";
        std::string language = get_lua_str(L, "compiler");
        if (language == "g++") language = "C++";
        else if (language == "gcc" || language == "clang") language = "C";
        std::unordered_map<std::string, std::string> str_keywords = {
            {"workspace", get_blmake_config_type(L)} ,
            {"location", get_lua_str(L, "out_dir")},
            {"project", get_blmake_config_type(L)},
            {"language", language},
            {"targetdir", get_lua_str(L, "out_dir")},
            {"objdir", get_lua_str(L, "out_dir")+"/obj"},
        };
        std::string prefix = get_lua_str(L, "src_dir");
        if (!prefix.empty()) prefix += "/";
        std::unordered_map<std::string, std::string> tbl_keywords = {
            {"files", get_lua_table_with_cmds_as_str(L, "files", prefix)} ,
            {"includedirs", get_lua_table_as_str(L, "include_dirs")},
            {"defines", get_lua_table_as_str(L, "preproc_opts")},
            {"libdirs", get_lua_table_as_str(L, "linker_opts")},
            {"links", get_lua_table_as_str(L, "dependencies")}, 
        };
        for (auto& [k, v] : str_keywords) {
            insert_field_str(lines, k, v);
        }
        for (auto& [k, v] : tbl_keywords) {
            insert_field_tbl(lines, k, v);
        }
        result = join(*lines, "\n");
        return result;
    }

    std::string populate_simple_build_premake_template(lua_State* L, const std::string& contents)
    {
        std::string result = contents;
        std::vector<std::string> temp_vec = split(result, "\n");
        std::vector<std::string>* lines = &temp_vec;
        std::string language = get_lua_str(L, "compiler");
        if (language == "g++") language = "C++";
        else if (language == "gcc" || language == "clang") language = "C";
        std::unordered_map<std::string, std::string> str_keywords = {
            {"workspace", get_blmake_config_type(L)} ,
            {"location", get_lua_str(L, "out_dir")},
            {"project", get_blmake_config_type(L)},
            {"language", language},
            {"targetdir", get_lua_str(L, "out_dir")},
            {"objdir", get_lua_str(L, "out_dir")+"/obj"},
        };
        std::unordered_map<std::string, std::string> tbl_keywords = {
            {"files", get_lua_table_with_cmds_as_str(L, "files", "")} ,
            {"includedirs", get_lua_table_as_str(L, "include_dirs")},
        };
        for (auto& [k, v] : str_keywords) {
            insert_field_str(lines, k, v);
        }
        for (auto& [k, v] : tbl_keywords) {
            insert_field_tbl(lines, k, v);
        }
        result = join(*lines, "\n");
        return result;
    }

    std::string populate_tiny_build_premake_template(lua_State* L, const std::string& contents)
    {
        std::string result = contents;
        std::vector<std::string> temp_vec = split(result, "\n");
        std::vector<std::string>* lines = &temp_vec;
        std::string language = get_lua_str(L, "compiler");
        if (language == "g++") language = "C++";
        else if (language == "gcc" || language == "clang") language = "C";
        std::unordered_map<std::string, std::string> str_keywords = {
            {"workspace", get_blmake_config_type(L)} ,
            {"location", get_lua_str(L, "out_dir")},
            {"project", get_blmake_config_type(L)},
            {"language", language},
            {"targetdir", get_lua_str(L, "out_dir")},
            {"objdir", get_lua_str(L, "out_dir")+"/obj"}, 
        };
        std::unordered_map<std::string, std::string> tbl_keywords = {
            {"files", get_lua_table_with_cmds_as_str(L, "files", "")} ,
        };
        for (auto& [k, v] : str_keywords) {
            insert_field_str(lines, k, v);
        }
        for (auto& [k, v] : tbl_keywords) {
            insert_field_tbl(lines, k, v);
        }
        result = join(*lines, "\n");
        return result;
    }

    std::string populate_test_build_premake_template(lua_State* L, const std::string contents)
    {
        std::string result = contents;
        std::vector<std::string> temp_vec = split(result, "\n");
        std::vector<std::string>* lines = &temp_vec;
        std::string tbl = get_lua_table_as_str(L, "lang_exts");
        std::string language = get_lua_str(L, "compiler");
        if (language == "g++") language = "C++";
        else if (language == "gcc" || language == "clang") language = "C";
        std::unordered_map<std::string, std::string> str_keywords = {
            {"workspace", get_blmake_config_type(L)} ,
            {"location", get_lua_str(L, "out_dir")},
            {"project", get_blmake_config_type(L)},
            {"language", language},
            {"targetdir", get_lua_str(L, "out_dir")},
            {"objdir", get_lua_str(L, "out_dir")+"/obj"},
        };
        std::string prefix = get_lua_str(L, "src_dir");
        std::string options = {
            get_lua_table_with_cmds_as_str(L, "warnings", "-W") + " " + 
            get_lua_str(L, "profiling")
        };
        options = remove_extra_spaces(options);
        if (!prefix.empty()) prefix += "/";
        std::string prebuild = "";
        std::string postbuild = "";
        lua_getfield(L, -1, "hooks");
        if (lua_istable(L, -1)) {
            lua_getfield(L, -1, "pre_build");
            if (lua_isstring(L, -1)) {
                prebuild = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
            lua_getfield(L, -1, "post_build");
            if (lua_isstring(L, -1)) {
                postbuild = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
        std::unordered_map<std::string, std::string> tbl_keywords = {
            {"files", get_lua_table_with_cmds_as_str(L, "files", prefix)} ,
            {"includedirs", get_lua_table_as_str(L, "include_dirs")},
            {"defines", get_lua_table_as_str(L, "preproc_opts")},
            {"buildoptions", options},
            {"libdirs", get_lua_table_as_str(L, "linker_opts")},
            {"links", get_lua_table_as_str(L, "dependencies")}, 
            {"prebuildcommands", prebuild}, 
            {"postbuildcommands", postbuild}, 
        };
        for (auto& [k, v] : str_keywords) {
            insert_field_str(lines, k, v);
        }
        for (auto& [k, v] : tbl_keywords) {
            insert_field_tbl(lines, k, v);
        }
        result = join(*lines, "\n");
        return result;
    }

    std::string populate_premake_template(lua_State* L, const std::string& contents)
    {
        std::string result = "";
        std::string kind = get_blmake_config_type(L);
        //todo: update the if-else pattern to a map to <string, function pointer>
        if (kind == "Full_build") {
            lua_getglobal(L, "Full_build");
            result = populate_full_build_premake_template(L, contents);
        }
        else if (kind == "Build") {
            lua_getglobal(L, "Build");
            result = populate_build_premake_template(L, contents);
        }
        else if (kind == "Simple_build") {
            lua_getglobal(L, "Simple_build");
            result = populate_simple_build_premake_template(L, contents);
        }
        else if (kind == "Tiny_build") {
            lua_getglobal(L, "Tiny_build");
            result = populate_tiny_build_premake_template(L, contents);
        }
        else if (kind == "Test_build") {
            lua_getglobal(L, "Test_build");
            result = populate_test_build_premake_template(L, contents);
        }
        return result;
    }
};

namespace Blmake
{
    size_t generate_full_build()
    {
        std::string contents = "";
        for (size_t i = 0; i < full_build_template_txt_len; i++) contents += full_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_build()
    {
        std::string contents = "";
        for (size_t i = 0; i < build_template_txt_len; i++) contents += build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_simple_build()
    {
        std::string contents = "";
        for (size_t i = 0; i < simple_build_template_txt_len; i++) contents += simple_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_tiny_build()
    {
        std::string contents = "";
        for (size_t i = 0; i < tiny_build_template_txt_len; i++) contents += tiny_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
    }

    size_t generate_test_build()
    {
        std::string contents = "";
        for (size_t i = 0; i < test_build_template_txt_len; i++) contents += test_build_template_txt[i];
        size_t err = write_string_to_file("./blmake.lua", contents);
        if (err == 0) std::cout << "Config file generated successfully!" << std::endl;
        return err;
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
            else if (check == std::string("premake")) {
                return 2;
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
};

namespace Watcher
{
    size_t generate_watcher_structure(lua_State* L)
    {
        std::string watcher_binary = (char*)src_watcher_watcher;
        std::string prev_json = reinterpret_cast<const char*>(src_watcher_prev_json);
        std::string src_path = "";
        std::string config_type = get_blmake_config_type(L);
        lua_getglobal(L, config_type.c_str());
        lua_getfield(L, -1, "src_dir");
        if (lua_isstring(L, -1)) {
            std::string src_dir = lua_tostring(L, -1);
            sanitize(src_dir);
            if (!src_dir.empty()) {
                src_path += src_dir + "/";
            }
        }
        lua_pop(L, 1);
        src_path += "watcher";
        std::filesystem::create_directory(src_path);
    #if OS_UNIX_LIKE_DEFINED
        size_t err1 = write_binary_data_to_file(src_path + "/watcher", src_watcher_watcher, src_watcher_watcher_len);
        std::string permissions_cmd = "chmod +x " + src_path + "/watcher";
        OS::run_command(permissions_cmd);
    #else
        size_t err1 = 0; //! temporary until Windows implementation works
    #endif
        size_t err2 = write_string_to_file(src_path + "/prev.json", prev_json);
        size_t err3 = write_string_to_file(src_path + "/recompile_list.txt", "");
        size_t any_err = err1 | err2 | err3;
        if (any_err == 0) std::cout << "Watcher structure generated successfully!" << std::endl;
        return any_err;
    }
};