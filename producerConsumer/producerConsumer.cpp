#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include<condition_variable>
#include "primes.cpp"


using namespace std;

bool done = false;
queue<int> list;
condition_variable cv;
mutex m;

void PrimeProducer (int *p){
    int primer = *p;
    int data = Primes::getNextPrime(primer);
    cout << "In producer" << endl;
    m.lock();
    list.push(data);
    m.unlock();
    cv.notify_all();
    cout << "I am a producer - I put: " << data << endl;
}

void PrimeConsumer (){
  cout << "In consumer" << endl;
  unique_lock<mutex> ul (m);
  cv.wait (ul, []{return list.size() > 0;});
  int data = list.front();
  list.pop();
  ul.unlock ();
  cout << "I am a consumer, I got: " << data << endl;
}

int main(void) {
   
    int firstPrime = 17;
    printf("Hi\n");
    thread producerThread(PrimeProducer, &firstPrime);
    thread consumerThread(PrimeConsumer);

  producerThread.join();
  consumerThread.join();
  return 0;
}
