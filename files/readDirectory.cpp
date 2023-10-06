#include <dirent.h>
#include <stdio.h>

int main (int argc, char * argv[])
{
    struct dirent * direntp;
    DIR * dirp = opendir(argv[1]);
    while ((direntp = readdir(dirp)) != NULL)
    {
        printf("%s\n", direntp->d_name);
    }
    closedir(dirp);
    return 0;
}