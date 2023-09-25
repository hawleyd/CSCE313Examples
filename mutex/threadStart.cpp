#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void threadFunc1()
{
    sleep(3);
    cout << "I'm One and my thread ID is: " << pthread_self() << endl;
    cout << "One finished " << endl;

}
void threadFunc2 (int x)
{
    sleep(x);
    cout << "I'm Two and my thread ID is: " << pthread_self() << endl;
    cout << "Two finished" << endl;
}
int main ()
{
    thread firstThread (threadFunc1);
    thread secondThread(threadFunc2,10);
    cout << "I'm main and my thread ID is: " << pthread_self() << endl;
    cout << "Main, First and Second are now concurrent" << endl;
    firstThread.join();    //wait to finish
    secondThread.join();    //wait to finish
    cout << "Everyone is finished" << endl;

}
