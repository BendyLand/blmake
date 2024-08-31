-- This will be the file where I plan the logic using lua directly (before translating to C++)

require "blmake"

function print_table(tbl)
    for key, val in pairs(tbl) do
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
end

function construct_full_compilation_command(tbl)
    result = tbl.compiler
    for _, opt in ipairs(tbl.preproc_opts) do
        result = string.format("%s %s", result, opt)
    end
    for _, ext in ipairs(tbl.lang_exts) do
        result = string.format("%s %s", result, ext)
    end
    if string.len(tbl.optimization) > 0 then
        result = string.format("%s %s", result, tbl.optimization)
    end
    if string.len(tbl.profiling) > 0 then
        result = string.format("%s %s", result, tbl.profiling)
    end
    for _, opt in ipairs(tbl.platform_opts) do
        result = string.format("%s %s", result, opt)
    end
    for _, dir in ipairs(tbl.include_dirs) do
        result = string.format("%s -I%s", result, dir)
    end
    prefix = tbl.src_dir
    for _, file in ipairs(tbl.files) do
        file = string.format("%s/%s", prefix, file)
        result = string.format("%s %s", result, file)
    end
    if string.len(tbl.lto) > 0 then
        result = string.format("%s %s", result, tbl.lto)
    end
    for _, opt in ipairs(tbl.linker_opts) do
        result = string.format("%s -L%s", result, opt)
    end
    for _, dep in ipairs(tbl.dependencies) do
        result = string.format("%s -l%s", result, dep)
    end
    for _, warning in ipairs(tbl.warnings) do
        result = string.format("%s -W%s", result, warning)
    end
    if tbl.debugging then
        result = result .. " -g"
    end
    for _, flag in ipairs(tbl.custom_flags) do
        result = string.format("%s -f%s", result, warning)
    end
    output = string.format("%s/%s", tbl.out_dir, tbl.output)
    result = string.format("%s -o %s", result, output)
    return result
end

test = construct_full_compilation_command(Full_build)
print(test)
