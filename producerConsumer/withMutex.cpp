/*

run this several times with high values of p and you will NOT get different values for p

*/

#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>

using namespace std;

void func(int *p, int x, mutex *m)
{
    m->lock();
    for (int i = 0; i<x;i++)
    {
        *p = *p+1;
    }
    cout << "My thread ID is: " << pthread_self() << " and p is " << *p << endl;
    m->unlock();
}

int main (int argc, char* argv[])
{
    int data = 0;
    int iterations = 0;
    mutex m;

    if (argc !=2)
    {
        printf ("Usage: %s 4", argv[0]);
    }
    else
    {
        iterations = atoi(argv[1]);
        thread firstThread(func, &data, iterations, &m);
        thread secondThread(func, &data, iterations, &m);
        cout << "Main, First and Second are now concurrent" << endl;
        firstThread.join();    //wait to finish
        secondThread.join();    //wait to finish
        cout << "Everyone is finished" << endl;
    }
    return 0;

}
