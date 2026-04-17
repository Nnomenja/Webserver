// g++ -Wall -pedantic -o cgi_runner cgi_runner.cpp
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>

// Helper to print error and exit
void fatal(const char* msg) {
    std::cerr << "ERROR: " << msg << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <cgi_script_path> [method] [query_string]" << std::endl;
        std::cerr << "Example: " << argv[0] << " ./test.cgi GET \"name=John&age=30\"" << std::endl;
        return EXIT_FAILURE;
    }

    const char* script_path = argv[1];
    const char* method = (argc >= 3) ? argv[2] : "GET";
    const char* query_string = (argc >= 4) ? argv[3] : "";

    // ----- Prepare CGI environment variables -----
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("REQUEST_METHOD", method, 1);
    setenv("QUERY_STRING", query_string, 1);
    setenv("SCRIPT_NAME", script_path, 1);   // simplified
    setenv("PATH_INFO", "", 1);
    setenv("CONTENT_LENGTH", "0", 1);        // no body for GET
    setenv("CONTENT_TYPE", "", 1);
    setenv("SERVER_SOFTWARE", "MiniCGI/1.0", 1);

    // ----- Create pipe to capture script's stdout -----
    int pipefd[2];
    if (pipe(pipefd) == -1)
        fatal("pipe");

    pid_t pid = fork();
    if (pid == -1)
        fatal("fork");

    if (pid == 0) {  // CHILD: execute CGI script
        close(pipefd[0]);                 // close read end
        dup2(pipefd[1], STDOUT_FILENO);   // redirect stdout to pipe
        close(pipefd[1]);

        // For a real server, you would also redirect stdin for POST requests.
        // Here we keep stdin unchanged (or close it for GET).
        if (strcmp(method, "POST") == 0) {
            // In a real server you'd set CONTENT_LENGTH and pipe the body.
            std::cerr << "POST body handling not implemented in this minimal example.\n";
        }

        execl(script_path, script_path, (char*)NULL);
        // If we reach here, exec failed
        std::cerr << "Failed to execute CGI script: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // PARENT: read child's output from pipe
    close(pipefd[1]);   // close write end

    char buffer[4096];
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        // Write the CGI output directly to our stdout.
        // In a real web server, you'd parse headers and then send the body.
        write(STDOUT_FILENO, buffer, n);
    }
    close(pipefd[0]);

    // Wait for child to finish
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        std::cerr << "CGI script exited with error code " << WEXITSTATUS(status) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

