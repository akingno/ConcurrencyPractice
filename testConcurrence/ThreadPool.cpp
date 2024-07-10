//
// Created by jacob on 2024/7/9.
//

#include "ThreadPool.h"
#include <mutex>
using namespace std;
ThreadPool::ThreadPool(int num_threads) {
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
  cout<<"clean over"<<endl;


}

void ThreadPool::EnqueueTask(shared_ptr<Task>& task) {
  /*
   *
   * 将task放入taskqueue
   * 用convar进行通知，再notify一个
   *
   * */
  {
    std::lock_guard<std::mutex> lock(mtx_queueMutex);
    pq_taskPriorityQueue.push(task);
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
    shared_ptr<Task> task;
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
        active_tasks.fetch_add(1);
      } else {
        continue;
      }
    }

    auto result = task->execute();

    cout<<"Result:"<<result<<endl;
    b_hasRun = true;
    active_tasks.fetch_sub(1);

    if (active_tasks.load() == 0) {
      std::lock_guard<std::mutex> lock(all_tasks_done_mutex);
      all_tasks_done.notify_one();
    }

  }
}
void ThreadPool::WaitForAllTasksDone() {
  unique_lock<mutex> lock(all_tasks_done_mutex);
  all_tasks_done.wait(lock,[this](){
    return (active_tasks.load() == 0 && b_hasRun.load());
  });
  cout<<"wait end"<<endl;

}
