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

#if defined(OS_UNIX_LIKE)
std::pair<int, std::string> OS::run_command_unix(const std::vector<std::string>& args) 
{
    // Convert std::vector<std::string> to an array of char* required by execvp
    std::vector<char*> cargs;
    for (const std::string& arg : args) {
        cargs.push_back(strdup(arg.c_str()));
    }
    cargs.push_back(nullptr);  // execvp expects a null-terminated array
    int pipefd[2];  // Pipe for capturing output
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return {-1, ""};
    }
    pid_t pid = fork();  // Fork the current process
    if (pid == -1) {
        // Error occurred during fork
        perror("fork failed");
        return {-1, ""};
    } 
    else if (pid == 0) {
        // Child process: redirect stdout to the pipe
        close(pipefd[0]);  // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe write end
        dup2(pipefd[1], STDERR_FILENO);  // Optionally redirect stderr to pipe write end
        close(pipefd[1]);  // Close original write end after duplication
        execvp(cargs[0], cargs.data());
        // execvp only returns if an error occurred
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } 
    else {
        // Parent process
        close(pipefd[1]);  // Close unused write end
        // Read output from the pipe
        std::string output;
        char buffer[128];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            output += buffer;
        }
        close(pipefd[0]);  // Close read end when done
        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);  // Wait for the child process to terminate
        // Free allocated memory for cargs
        for (char* carg : cargs) {
            free(carg);
        }
        // Return child's exit status and captured output
        if (WIFEXITED(status)) {
            return {WEXITSTATUS(status), output};  // Return exit code and output
        } 
        else {
            return {-1, output};  // Child did not exit normally, return -1 with captured output
        }
    }
}
#endif

std::pair<int, std::string> OS::run_command(std::string& arg) 
{
    std::pair<int, std::string> result;
    std::vector<std::string> args = split(arg, ' ');
    #if defined(OS_UNIX_LIKE)
        result = OS::run_command_unix(args);
    #else
        result = OS::run_command_windows(arg);
    #endif
    return result;
}

//todo: check this implementation
#if defined(OS_WINDOWS)
static std::pair<int, std::string> OS::run_command_windows(const std::string& command) 
{
    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo = { sizeof(startupInfo) };
    SECURITY_ATTRIBUTES securityAttrs = { sizeof(securityAttrs), NULL, TRUE };
    HANDLE hRead, hWrite;
    // Create pipe for capturing output
    CreatePipe(&hRead, &hWrite, &securityAttrs, 0);
    startupInfo.hStdOutput = hWrite;
    startupInfo.hStdError = hWrite;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    // Execute command
    if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo)) {
        return {-1, "Failed to create process"};
    }
    // Wait for process to complete and capture output
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(hWrite);
    // Read pipe output
    char buffer[128];
    std::string output;
    DWORD bytesRead;
    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        output += buffer;
    }
    CloseHandle(hRead);
    // Get exit code
    DWORD exitCode;
    GetExitCodeProcess(processInfo.hProcess, &exitCode);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return {exitCode, output};
}
#endif