//
// Created by jacob on 2024/7/9.
//

#include "ThreadPool.h"
using namespace std;
ThreadPool::ThreadPool() {
  b_stopFlag = false;
}
ThreadPool::~ThreadPool() {
}
void ThreadPool::NotifiedByTaskqueue() {
}
void ThreadPool::DispatchTask() {
  /*
   * 寻找是否有空的Thread
   * 如果有，唤醒这个Thread的run
   *
   *
   * */
}
void ThreadPool::Stop() {
}
void ThreadPool::EnqueueTask(Task task) {
}
void ThreadPool::AddingThreads(int num_threads) {
  for (int i = 0; i < num_threads; ++i){
    m_threads.emplace_back(make_shared<ThreadStruct>(shared_from_this())); //todo：此处调用ThreadStruct构造函数但还未实现
  }
  for(auto& thread_struct : m_threads){
    thread_struct->m_thread = std::thread(&ThreadStruct::Run, thread_struct);
  }
}
void ThreadPool::Init() {
}
bool ThreadPool::IsFull() const {
  for(auto& each_thread : m_threads){
    if(each_thread->b_isInLeisure){
      return false;
    }
  }
  return true;
}



ThreadPool::ThreadStruct::ThreadStruct(shared_ptr<ThreadPool> pool) {
}
ThreadPool::ThreadStruct::~ThreadStruct() {
}
void ThreadPool::ThreadStruct::Run() {
  while(true){
    /*
     * 1. con_var.wait()
     * 2. 如果
     *
     *
     * */
    Task task;
    {
      unique_lock<mutex> lock(m_condition_mutex);

      con_Var.wait(lock,[this](){
        auto poolPtr = m_threadPool.lock();
        return (!poolPtr->pq_taskPriorityQueue.empty() || poolPtr->b_stopFlag);
      });

      auto poolPtr = m_threadPool.lock();

      if(!poolPtr){
        return;
      }
      if(poolPtr->pq_taskPriorityQueue.empty()&& poolPtr->b_stopFlag){
        return;
      }
      if (!poolPtr->pq_taskPriorityQueue.empty()) {
        auto taskWithPriority = poolPtr->pq_taskPriorityQueue.top();
        poolPtr->pq_taskPriorityQueue.pop();
      } else {
        continue;
      }
    }
    task.execute();
    b_isInLeisure = true;
  }
}
void ThreadPool::ThreadStruct::Stop() {
  if(m_thread.joinable()){
    m_thread.join();
  }
}
void ThreadPool::ThreadStruct::Notify() {
}
