#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <wait.h>

using namespace std;


int main (){
    char c = 'k';
    int fd1 = open("foobar.txt", O_RDONLY, 0);
    int fd2 = open("foobar.txt", O_RDONLY, 0);
    if(fork()==0)
    {
        //child
        cout << "Child has:" << c << endl;
        read(fd1, &c, 1);
        cout << "Child read fd1:" << c << endl;
        read(fd2, &c, 1);
        cout << "Child read fd2:" << c << endl;
    }
    else
    {
        //parent
        wait(NULL);
        cout << "Parent has:" << c << endl;
        read(fd1, &c, 1);
        cout << "Parent read fd1:" << c << endl;
        read(fd2, &c, 1);
        cout << "Parent read fd2:" << c << endl;
    }
}
