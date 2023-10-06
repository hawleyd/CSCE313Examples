#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void my_handler (int sig){
    cout << "I got a signal: " << getpid() << endl;
    wait (0);
}

int main (){
    // install handler
    signal (SIGCHLD, my_handler);
    
    // create 5 child processes. 
    // ith process sleeps for i sec and then dies
    for (int i=1; i<=5; i++){
        int pid = fork ();
        if (pid == 0){
            cout << "Night: " << getpid() << endl;
            sleep (i*4);
            cout << "Up and out: " << getpid() << endl;
            return 0; 
        }
    }
    // parent in an infinite loop
    // busy doing something else
    while (true){
        cout << "Relaxing" << endl;
        sleep (1);
    }
}
