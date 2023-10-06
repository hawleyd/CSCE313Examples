/*

run this several times with high values of p and you will get different values for p

*/

#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void func(int *p, int x)
{
    for (int i = 0; i<x;i++)
    {
        *p = *p+1;
    }
    cout << "My thread ID is: " << pthread_self() << " and p is " << *p << endl;

}

int main (int argc, char* argv[])
{
    int data = 0;
    int interations;

    if (argc !=2)
    {
        printf ("Usage: noMutex 4");
    }
    else
    {
        interations = atoi(argv[1]);
        thread firstThread(func, &data, interations);
        thread secondThread(func, &data, interations);
        cout << "Main, First and Second are now concurrent" << endl;
        firstThread.join();    //wait to finish
        secondThread.join();    //wait to finish
        cout << "Everyone is finished" << endl;
    }
    return 0;

}
