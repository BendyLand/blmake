#include "os.hpp"

std::string OS::detect_os() 
{
    std::string result;
    #if defined(OS_WINDOWS)
        result = "Windows";
    #elif defined(OS_MACOS)
        result = "MacOS";
    #elif defined(OS_LINUX)
        result = "Linux";
    #elif defined(OS_UNIX)
        result = "Unix";
    #elif defined(OS_FREEBSD)
        result = "FreeBSD";
    #else
        result = "Unknown";
    #endif
    return result;
}

int OS::run_command_unix(const std::vector<std::string>& args)
{
    // Convert std::vector<std::string> to an array of char* required by execvp
    std::vector<char*> cargs;
    for (std::string arg : args) {
        cargs.push_back(strdup(arg.c_str()));
    }
    cargs.push_back(nullptr); // execvp expects a null-terminated array

    pid_t pid = fork();  // Fork the current process
    if (pid == -1) {
        // Error occurred during fork
        perror("fork failed"); 
        return -1;
    } 
    else if (pid == 0) {
        // Child process: execute the command
        execvp(cargs[0], cargs.data());
        // execvp only returns if an error occurred
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent process: wait for the child to finish
        int status;
        waitpid(pid, &status, 0);  // Wait for the child process to terminate
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);  // Return child's exit status
        } 
        else {
            return -1;  // Child did not exit successfully
        }
    }
}

int OS::run_command(std::string& arg) 
{
    int result;
    std::vector<std::string> args = split(arg, ' ');
    #if defined(OS_UNIX_LIKE)
        result = OS::run_command_unix(args);
    #else
        std::cerr << "`run_command` is not implemented for Windows yet." << std::endl;
        result = -1;
    #endif
    return result;
}
