#include <stdio.h>
#include <unistd.h>

int main()
{
    if (unlink("yetAnother.txt") == -1)
    {
        perror("failed to delete link");
    }
    else
    {
        printf("Unlinked");
    }
    return 0;
}