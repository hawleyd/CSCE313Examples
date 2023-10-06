#include <stdio.h>
#include <unistd.h>

int main()
{
    if (link("anotherFoobar.txt", "yetAnother.txt") == -1)
    {
        perror("failed to make new link");
    }
    else
    {
        printf("Linked");
    }
    return 0;
}