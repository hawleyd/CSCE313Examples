#include <iostream>
#include <queue>
#include <unistd.h>
#include "primes.cpp"
#include "semaphore.cpp"


using namespace std;

bool done = false;
std::queue<int> list;
Semaphore s(0);

void PrimeProducer (int *p){
    int primer = *p;
    int data = Primes::getNextPrime(primer);
    cout << "In producer" << endl;
    list.push(data);
    s.post();
    cout << "I am a producer - I put: " << data << endl;
}

void PrimeConsumer (){
  cout << "In consumer" << endl;
  s.wait();
  int data = list.front();
  list.pop();
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
