

#include "MyTask.h"
#include "Task.h"
#include "ThreadPool.h"
#include "stdafx.h"

using namespace std;




auto SplitRange(const int low,const int high,int num_threads) {
  /*
   * 把求和范围切割成 2 * threads 份
   * */
  int subRangeLength = ((high - low) + 1) / (2 * num_threads);

  auto subRange = vector<pair<int,int>>{};

  for(int i = low; i < high;){
    if(i + subRangeLength <= high){
      subRange.emplace_back(i,i+subRangeLength-1);
      i += subRangeLength;
    }
    else{
      subRange.emplace_back(i,high);
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
 *
 * 用多线程求两个数中间的素数之和
 *
 * */
int main() {
  int NUM_THREADS = 4;

  condition_variable AccumulateVariable;

  atomic<long long> Sum =0;

  auto [low, high] = LoadInput();

  /*
   *
   * 读取计算范围后开始计时
   *
   * */
  auto start = chrono::system_clock::now();

  /*
   * subRange中，[lowest, highest]
   *
   * */
  auto subRange = SplitRange(low, high, NUM_THREADS);



  /*
   *
   * 创建线程池
   *
   * */
  auto threadPool = ThreadPool::CreateThreadPool(NUM_THREADS);


  /*
   *
   * 把分割好的范围一段段和 taskFunction组合好，并放入taskqueue
   *
   * */
  for(auto subrange : subRange) {

    /*
     * 把taskAccumulate函数和其参数
     * 封装进std::function
     *
     * */
    auto functionTaskAccumulate = std::function<void()>([=, &Sum,&threadPool](){
      long long sum = 0;
      for (int i = subrange.first; i <= subrange.second; ++i){
        if(Is_prime(i)){
          sum+=i;
          //cout<<","<<i;
        }
      }

      Sum += sum;
      threadPool->EnqueueResult(sum);
    });

    auto task = make_shared<Task<void>>(functionTaskAccumulate,0);
    threadPool->EnqueueTask(task);

  }
  /*
   *
   * 在放入taskqueue后，开始阻塞等待，直到所有thread执行完毕
   *
   * */
  threadPool->WaitForAllTasksDone();
  cout<<"Total By Sum:"<<Sum<<endl;
  auto end = chrono::system_clock::now();

  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  cout<<"Time spent:"
       << static_cast<double>(duration.count()) / 1000.0
       <<" milliseconds"<<endl;



  return 0;
}


