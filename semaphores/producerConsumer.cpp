#include <iostream>
#include <queue>
#include<condition_variable>
#include "primes.cpp"

using namespace std;

bool done = false;
queue<int> list;
condition_variable cv;
mutex m;

void Producer (){
    int data = getdatafrmsrc();
    m.lock();
    list.push_back (data);
    m.unlock();
    cv.notify_all();
    cout << "I am a producer - I put: " << data << endl;
}
void Consumer (){
  unique_lock<mutex> ul (m);
  cv.wait (ul, []{return list.size() > 0;});
  int data = list.front();
  list.pop();
  ul.unlock ();
  cout << "I am a consumer, I got: " << data << endl;
}

int main() {
   
    int currentPrime;

    std::cout << "Enter a prime number: ";
    std::cin >> currentPrime;

    int next = Primes::getNextPrime(currentPrime);
    std::cout << "The next prime number after " << currentPrime << " is " << next << std::endl;

    return 0;
}
