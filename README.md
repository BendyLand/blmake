# blmake

*This project is in its fairly early stages as of Sep. 7, 2024. Functionality is limited and unpredictable.*

A simple and straightforward build tool for C/C++!

## About

`blmake` is the third and final project in my series of build tools. The other two (`blrun` and `blbld`), are meant for smaller projects, while blmake is designed for larger, more complex projects. Although blmake has a steeper learning curve, it offers more functionality for situations where the other tools fall short, yet is still far simpler than traditional build tools. Here is a quick overview of all three tools:
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
     - Use for projects that would normally warrant the usage of something like CMake.
     - Rather than defining its own custom syntax, `blmake` will expect a lua file called "blmake.lua".
     - There [will be] commands to generate the config file and run various parts of the build process.
     - *Actual commands TBD*
