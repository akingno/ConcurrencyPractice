//
// Created by jacob on 2024/7/9.
//

#include "ThreadPool.h"
#include <mutex>
using namespace std;
ThreadPool::ThreadPool(int num_threads) {
  b_stopFlag = false;
  Init(num_threads);
}
ThreadPool::~ThreadPool() {
  /*
   *
   * 停止并删除所有threadstruct
   *
   * */
  Stop();
}

void ThreadPool::Stop() {
  /*
   *
   * 所有thread，能join都join
   * b_stopFlag = true
   *
   * */

  {
    std::lock_guard<std::mutex> lock(mtx_queueMutex);
    b_stopFlag = true;
  }

  con_Var.notify_all();

  for(auto& thd:m_threads){
    if(thd.joinable()){
      thd.join();
    }
  }

  m_threads.clear();
}

void ThreadPool::EnqueueTask(Task&& task) {
  /*
   *
   * 将task放入taskqueue
   * 用convar进行通知，再notify一个
   *
   * */
  {
    std::lock_guard<std::mutex> lock(mtx_queueMutex);
    pq_taskPriorityQueue.push(std::move(task));
    cout<<"taskPQ size:"<<pq_taskPriorityQueue.size()<<endl;
  }
  con_Var.notify_one();

}



void ThreadPool::Init(int num_threads) {
  /*
   * 初始化，Adding threads
   *
   * */
  for(int i = 0; i < num_threads; ++i){
    m_threads.emplace_back(&ThreadPool::Run,this);
  }

  //AddingThreads(num_threads);
}

bool ThreadPool::IsFull() const {
  /*
   *
   * 判断是否pool中线程满了
   * TODO:其实我感觉这个函数useless
   *
   * */
  return true;
}


void ThreadPool::Run() {
  while(true){
    /*
     * 1. con_var.wait()
     * 2. 如果
     *
     *
     * */
    Task task;
    {
      unique_lock<mutex> lock(mtx_queueMutex);

      con_Var.wait(lock,[this](){
        return (!pq_taskPriorityQueue.empty() || b_stopFlag);
      });


      if(pq_taskPriorityQueue.empty()&& b_stopFlag){
        return;
      }
      if (!pq_taskPriorityQueue.empty()) {
        task = pq_taskPriorityQueue.top();
        pq_taskPriorityQueue.pop();
      } else {
        continue;
      }
    }

    auto result = task.execute();

    cout<<"Result:"<<result<<endl;

  }
}

