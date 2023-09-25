#include <iostream>
#include <queue>
#include <unistd.h>
#include "primes.cpp"
#include "semaphore.cpp"


using namespace std;

bool done = false;
int buf_size=20;
int nextPrime = 1;
std::queue<int> list;

Semaphore fullSlots(0);
Semaphore emptySlots(buf_size);
Semaphore syncPrimes(1);

void PrimeProducer (int *p){
    int primer = *p;
    int data;

    if (primer > nextPrime)
    {
      nextPrime = primer;
    }
    cout << "In producer [" << pthread_self() << "] with " << nextPrime << endl;

    syncPrimes.wait();
    emptySlots.wait();

    data = Primes::getNextPrime(nextPrime);
    list.push(data);
    nextPrime = data;

    syncPrimes.post();
    fullSlots.post();

    cout << "Out of producer [" <<pthread_self() << "] - I put: " << data << endl;
}

void PrimeConsumer (){
  cout << "In consumer [" << pthread_self() << "]" << endl;

  fullSlots.wait();
  syncPrimes.wait();

  int data = list.front();
  list.pop();
  syncPrimes.post();
  emptySlots.post();

  cout << "Out of consumer [" <<pthread_self() << "] - I got: " << data << endl;
  
}

int main(int argc, char* argv[]) {
   
  int opt;

  int i;
  int nextNum;
  thread prod_threads[1000];
  thread cons_threads[1000];
 
  if (argc !=3)
  {
        printf ("Usage: %s -n prime_number\n", argv[0]);
  }
  else
  {
    while ((opt = getopt(argc, argv, "n:")) != -1) {
      switch (opt) {
        case 'n':
          nextNum = atoi(optarg);
          break;
      }
    }
    cout << "In main - first number is " << nextNum << "\n";
    i = 0;
    
    while (i < 10)
    {
      
      prod_threads[i] = thread(PrimeProducer, &nextNum);
      cons_threads[i] = thread(PrimeConsumer);
      i++;
    }

    for(int j = 0; j < i; j++)
    {
      prod_threads[j].join();
      cons_threads[j].join();
    }
  }
  return 0;
}
