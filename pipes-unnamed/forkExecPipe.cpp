#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    
    int status;

    int pipefds[2];
    char buf[30];

    if (pipe(pipefds) ==-1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    memset(buf,0,30);

    pid_t child_pid = fork();

    if (child_pid == -1) {
        std::cerr << "Fork failed." << std::endl;
        return 1;
    }

    if (child_pid == 0) { // Child process
        std::cout << "Hello from the child process!\nThe parent process ID is " << getppid() << std::endl;

        std::cout << "Child is writing in the pipe" << std::endl;
        close (pipefds[0]);     //close read end
        write(pipefds[1], "I'm your favorite child", 24);
        close(pipefds[0]);      //close write end

        sleep(1); // Sleep for 6 seconds
        execl("/bin/ls", "ls", "-l", nullptr); // Execute ls -l
        exit(0); // Exit normally
    } 
    else { // Parent process

        wait(&status);

        close(pipefds[1]);  //close write end
        read(pipefds[0], buf, 24);
        close(pipefds[0]);  //close read end

        std::cout << "Hello from the parent process!\n";
        std::cout << "Parent received: " << buf << std::endl;
        std::cout << "Child  process ID was " << child_pid << std::endl;        
        
        if (WIFEXITED(status)) {
            std::cout << "Child process exited normally with status: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Child process terminated by signal: " << WTERMSIG(status) << std::endl;
        }  
    }

    return 0;
}
