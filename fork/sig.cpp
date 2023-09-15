// C++ Program to demonstrate
// a multiprocessing environment.
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main()
{
	pid_t c_pid = fork();

	if (c_pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (c_pid > 0) {
		signal(SIGCHLD, SIG_IGN);
		cout << "printed from parent process " << getpid()
			<< endl;
	}
	else {
		cout << "printed from child process " << getpid()
			<< endl;
	}

	return 0;
}
