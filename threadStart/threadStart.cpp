#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void threadFunc1()
{
    sleep(3);
    cout << "One finished " << endl;

}
void threadFunc2 (int x)
{
    sleep(x);
    cout << "Two finished" << endl;
}
int main ()
{
    thread firstThread (threadFunc1);
    thread secondThread(threadFunc2,10);
    cout << "Main, First and Second are now concurrent" << endl;
    firstThread.join();    //wait to finish
    secondThread.join();    //wait to finish
    cout << "Everyone is finished" << endl;

}
