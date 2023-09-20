#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    int fd1, fd2, fd3;
    char *fname = argv[1];
    fd1 = open(fname, O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
    write(fd1, "pqrs", 4);
    fd2 = open(fname, O_APPEND|O_WRONLY, 0);
    write(fd2, "jklmn", 5);
    fd3 = dup(fd1);  /* Allocates descriptor */
    write(fd3, "wxyz", 4);
    write(fd2, "ef", 2);
    // playing with file position
    lseek (fd1, 0, SEEK_SET);
    write (fd1, "ab", 2);
    return 0;
}

