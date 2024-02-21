#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    

    pid_t child_pid = fork();

    if (child_pid == -1) {
        std::cerr << "Fork failed." << std::endl;
        return 1;
    }

    if (child_pid == 0) { // Child process
        std::cout << "Hello from the child process!\nThe parent process ID is " << getppid() << std::endl;

        sleep(1); // Sleep for 6 seconds
        execl("/bin/ls", "ls", "-l", nullptr); // Execute ls -l
        exit(0); // Exit normally
    } 
    else { // Parent process
        wait(NULL);
        std::cout << "Hello from the parent process!\n";
        std::cout << "Child  process ID was " << child_pid << std::endl;

        int status;
        
        if (WIFEXITED(status)) {
            std::cout << "Child process exited normally with status: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Child process terminated by signal: " << WTERMSIG(status) << std::endl;
        }  
    }

    return 0;
}
