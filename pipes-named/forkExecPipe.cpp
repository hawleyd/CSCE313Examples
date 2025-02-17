#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cassert>

int main() {
    int status;
    
    //MY_FIFO already exists
    const char *FIFO =  "/tmp/MY_FIFO";

    int fifo;
    char buf[30];
    memset(buf, 0, 30);

    pid_t child_pid = fork();

    if (child_pid == -1) {
        std::cerr << "Fork failed." << std::endl;
        return 1;
    }

    if (child_pid == 0) { // Child process
        sleep(1); // Ensure parent opens FIFO first

        fifo = open(FIFO, O_WRONLY);
        assert(fifo != -1);

        std::cout << "Hello from the child process!\n";
        std::cout << "The parent process ID is " << getppid() << std::endl;

        std::cout << "Child is writing in the pipe" << std::endl;
        write(fifo, "I'm your favorite child", 23);
        close(fifo);

        execl("/bin/ls", "ls", "-l", nullptr);
        exit(0);
    } 
    else { // Parent process
        fifo = open(FIFO, O_RDONLY); // Open FIFO before waiting for the child
        assert(fifo != -1);

        wait(&status);

        read(fifo, buf, 23);
        close(fifo);

        std::cout << "Hello from the parent process!\n";
        std::cout << "Parent received: " << buf << std::endl;
        std::cout << "Child process ID was " << child_pid << std::endl;        
        
        if (WIFEXITED(status)) {
            std::cout << "Child process exited normally with status: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Child process terminated by signal: " << WTERMSIG(status) << std::endl;
        }

    }

    return 0;
}
