#include <iostream>
#include <queue>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
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

int main(int argc, char* argv[]) {
   
  int opt;
  int firstPrime;
  if (argc !=3)
  {
        printf ("Usage: %s -n prime_number\n", argv[0]);
  }
  else
  {
    while ((opt = getopt(argc, argv, "n:")) != -1) {
      switch (opt) {
        case 'n':
          firstPrime = atoi(optarg);
          break;
      }
    }
    printf("In main\n");
    thread producerThread(PrimeProducer, &firstPrime);
    //thread producerThread2(PrimeProducer, &firstPrime);
    thread consumerThread(PrimeConsumer);

    producerThread.join();
    //producerThread2.join();
    
    consumerThread.join();
  }
  return 0;
}
