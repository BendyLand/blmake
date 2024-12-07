# blmake

*This project is in a somewhat early stage as of Oct. 31, 2024. Functionality may be unpredictable, but basic builds should actually work on Unix-like systems. However, as the tool is still being built, there is no usage guide yet.*

A simple and straightforward build tool for C/C++!

## About

`blmake` is the third and final project in my series of build tools. The other two (`blrun` and `blbld`), are meant for smaller projects in various languages, while blmake is designed for larger, more complex projects primarily in C/C++. Although blmake has a steeper learning curve, it offers more functionality for situations where the other tools fall short, yet is still far simpler than traditional build tools. Here is a quick overview of all three tools:
 - `blrun`: 
     - The smallest of the build tools. 
     - Use for small compiled projects that don't include external dependencies. 
     - There may be several files.
     - There is only a command to compile and run the project.
 - `blbld`: 
     - A moderate sized build tool. 
     - Use for projects that may have extra includes (-I) or for projects with a slightly more in depth directory structure. 
     - There may be several files and basic dependencies, like an extra Include path.
     - There are more commands to compile individual files, compile everything to an object file (.o), compile everything directly, etc. 
 - `blmake` (this project):
     - The largest of the build tools.
     - Use for projects that would normally warrant the usage of something like Make.
     - Rather than defining its own custom syntax, `blmake` will expect a Lua file called "blmake.lua".
     - There are commands to generate this config file. 
       - The command to generate a config file is `blmake gen <option>`.
       - The options are: `full`, `build`, `simple`, `tiny`, and `test`. Leaving the option blank will default to `full`.
     - There are commands to generate a file watcher for your project using a blmake.lua config file.
       - The command to generate the watcher directory is `blmake watch`.
       - The build tool will automatically run incremental builds if it detects the presence of a watcher directory.
       - The file watcher depends on a file it generates called 'prev.json'. If the data kept in this file becomes corrupted, it will no longer function properly. To reset 'prev.json', run `blbld watch clean`.
     - *Build commands TBD*
     - *Note: The command `blmake` is not available by default on your system. You will need to build the project, rename the binary, and give it executable permissions to use it in this way.*
