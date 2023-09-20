#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    char c1, c2, c3;
    char *fname = "file.txt";
    int fd1 = open(fname, O_RDONLY);
    int fd2 = open(fname, O_RDONLY);
    int fd3 = open(fname, O_RDONLY);
    dup2(fd2, fd3);
    read(fd1, &c1, 1);
    read(fd2, &c2, 1);
    read(fd3, &c3, 1);
    printf("c1 = %c, c2 = %c, c3 = %c\n", c1, c2, c3);
}
