//
// Created by jacob on 2024/7/9.
//

#include "ThreadPool.h"
#include <mutex>
using namespace std;
ThreadPool::ThreadPool(int num_threads) :b_stopFlag(false), b_hasRun(false){
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
  //cout<<"clean over"<<endl;


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
    //cout<<"taskPQ size:"<<pq_taskPriorityQueue.size()<<endl;
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


void ThreadPool::Run() {
  while(true){

    shared_ptr<Task> task;
    {
      unique_lock<mutex> lock(mtx_queueMutex);

      /*
       *
       * 如果taskpq有任务或者已经结束了就解锁
       *
       * */
      con_Var.wait(lock,[this](){
        return (!pq_taskPriorityQueue.empty() || b_stopFlag);
      });

      /*
       *
       * 如果已经结束了而且taskpq空了则返回
       *
       * */
      if(pq_taskPriorityQueue.empty() && b_stopFlag){
        return;
      }
      /*
       *
       * 如果taskqp有task则取出执行
       *
       * */
      if (!pq_taskPriorityQueue.empty()) {
        task = pq_taskPriorityQueue.top();
        pq_taskPriorityQueue.pop();
        active_tasks.fetch_add(1);
      } else {
        continue;
      }
    }

    task->execute();
    b_hasRun = true;
    active_tasks.fetch_sub(1);

    /*
     *
     * 执行完，如果此时没有任何task执行，通知main进入下一步
     *
     * */
    if (active_tasks.load() == 0) {
      std::lock_guard<std::mutex> lock(all_tasks_done_mutex);
      all_tasks_done.notify_one();
    }

  }
}
void ThreadPool::WaitForAllTasksDone() {
  /*
   *
   * main在新建完task后会开始执行这个函数，等所有tasks都执行完再进行下一步
   *
   * */
  unique_lock<mutex> lock(all_tasks_done_mutex);
  all_tasks_done.wait(lock,[this](){
    return (active_tasks.load() == 0 && b_hasRun.load());
  });


}

