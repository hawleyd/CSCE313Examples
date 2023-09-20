#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;


int main (){
    char c;
    int fd1 = open("foobar.txt", O_RDONLY, 0);
    int fd2 = open("foobar.txt", O_RDONLY, 0);
    read(fd1, &c, 1);
    cout << c << endl;
    read(fd2, &c, 1);
    cout << c << endl;
}
