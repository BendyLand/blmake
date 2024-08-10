# blmake

A simple and straightforward build tool for C/C++!

## About

This is the third and final project of mine relating to build tools. The other two `blrun` and `blbld` both have their own uses, both of which are for smaller projects than this one expects. As a full-sized build tool, `blmake` has a steeper learning curve, but offers more functionality when the other tools are not sufficient. Here is a quick overview of the useage of these tools:
 - `blrun`: 
     - The smallest of the build tools. 
     - Use for small compiled projects that don't include external dependencies. 
     - There may be several files.
     - There is only a command to compile and run the project.
 - `blbld`: 
     - A moderate sized build tool. 
     - Use for projects that may have extra includes (-I) or for projects with a slightly more in  depth directory structure. 
     - There may be several files and basic dependencies, like an extra Include path.
     - There are more commands to compile individual files, compile everything to an object file (.o), compile everything directly, etc. 
 - `blmake` (this project):
     - The largest of the build tools.
     - Use for projects that would normally warrant the usage of something like CMake.
     - *Commands TBD*
     - Rather than defining its own custom syntax, `blmake` expects a lua file called "blmake.lua".
