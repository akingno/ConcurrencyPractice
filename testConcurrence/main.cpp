
#include "Globals.h"
#include "MyTask.h"
#include "Task.h"
#include "ThreadPool.h"
#include "stdafx.h"

using namespace std;




auto SplitRange(const int low,const int high) {
  /*
   * 切割成 2 * threads 份
   * */
  int subRangeLength = ((high - low) + 1) / (2 * Globals::NUM_THREADS);
  auto subRange = make_shared<vector<pair<int,int>>>();
  for(int i = low; i < high;){
    if(i + subRangeLength <= high){
      subRange->emplace_back(i,i+subRangeLength-1);
      i += subRangeLength;
    }
    else{
      subRange->emplace_back(i,high);
      break;
    }
  }
  /*
   * Split End
   * */

  return subRange;
}

/*
 *
 * 判断是否是素数
 *
 * */
bool Is_prime(int n) {
  if (n <= 1) return false;
  for (int i = 2; i * i <= n; ++i) {
    if (n % i == 0) return false;
  }
  return true;
}

/*
 *
 * 输入求素数范围
 *
 * */
tuple<int, int> LoadInput(){
  int low,high;
  cout << "Please enter two numbers" << endl;
  cin >> low >> high;
  return make_tuple(low,high);
}

/*
 * 用多线程求两个数中间的素数之和
 *
 * TODO: 1. 实现主线程读取任务切割任务和输出结果。
 *        2.实现线程池和线程的初始化，对线程的通知，和销毁线程。
 *        3.实现任务队列，实现主线程把任务放入任务队列，和通知线程池。
 *        4.实现线程池的任务分配（线程通知线程池：
 *          每个线程在完成任务后，会通知线程池进行任务分配。
 *          线程池会从任务队列中取出新任务并分配给通知的线程。）
 *
 * */
int main() {

  atomic<long long> Sum =0;
  auto [low, high] = LoadInput();
  auto start = chrono::system_clock::now();

  /*
   * subRange中，[lowest, highest]
   *
   * */
  auto subRange = SplitRange(low, high);


  /*
   *
   * taskAccumulate 把一个求和函数封进lambda
   *
   *
   *
   * */

  auto taskAccumulate = [](pair<int,int> range, atomic<long long>& Sum){
    long long sum = 0;
    for (int i = range.first; i <= range.second; ++i){
      if(Is_prime(i)){
        sum+=i;
        //cout<<","<<i;
      }
    }

    Sum += sum;
  };



  auto threadPool = make_unique<ThreadPool>(Globals::NUM_THREADS);
  for(auto subrange : *subRange) {

    /*
     * 把taskAccumulate函数和其参数
     * 封装进std::function
     *
     * */
    auto packagedTaskAccumulate = std::function<void()>([=, &Sum](){
      taskAccumulate(subrange, Sum);
    });

    auto task = make_shared<Task<void>>(packagedTaskAccumulate,0);
    threadPool->EnqueueTask(task);

  }
  threadPool->WaitForAllTasksDone();
  cout<<"Total:"<<Sum<<endl;
  auto end = chrono::system_clock::now();

  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  cout<<"Time spent:"
       << static_cast<double>(duration.count()) / 1000.0
       <<" milliseconds"<<endl;



  return 0;
}


