#include <iostream>
#include <queue>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore{
private:
  int value;
  std::mutex m;
  std::condition_variable cv;
public:
  Semaphore (int _v):value(_v){}
  void wait(){
    unique_lock<mutex> l (m);
    // wait until the value is positive
    cv.wait (l, [this]{return value > 0;});
    value --;
  }
  void post(){
    unique_lock<mutex> l (m);
    value ++;
    cv.notify_one();
  }
};
