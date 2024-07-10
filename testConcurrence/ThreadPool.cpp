//
// Created by jacob on 2024/7/9.
//

#include "ThreadPool.h"
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
void ThreadPool::NotifiedByTaskqueue() {
}
void ThreadPool::DispatchTask() {
  /*
   * 寻找是否有空的Thread
   * 如果有，Notify it(让其自行解锁)
   *
   *
   * */
  for(const auto& threadStruct : m_threads){
    if(threadStruct->b_isInLeisure){
      threadStruct->Notify();
      return;
    }
  }
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

  con_condition.notify_all();

  for(auto& threadStruct : m_threads){
    if(threadStruct->b_isInLeisure){
      threadStruct->Stop();
    }
  }
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
  }
  DispatchTask();
}

void ThreadPool::AddingThreads(int num_threads) {
  for (int i = 0; i < num_threads; ++i){
    m_threads.emplace_back(make_shared<ThreadStruct>(shared_from_this())); //todo：此处调用ThreadStruct构造函数但还未实现
  }
  for(auto& thread_struct : m_threads){
    thread_struct->m_thread = std::thread(&ThreadStruct::Run, thread_struct);
  }
}

void ThreadPool::Init(int num_threads) {
  /*
   * 初始化，Adding threads
   *
   * */
  AddingThreads(num_threads);
}
bool ThreadPool::IsFull() const {
  /*
   *
   * 判断是否pool中线程满了
   * TODO:其实我感觉这个函数useless
   *
   * */
  for(auto& each_thread : m_threads){
    if(each_thread->b_isInLeisure){
      return false;
    }
  }
  return true;
}



ThreadPool::ThreadStruct::ThreadStruct(shared_ptr<ThreadPool> pool) :m_threadPool(pool){
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
      unique_lock<mutex> lock(mtx_conditionMutex);

      con_Var.wait(lock,[this](){
        auto poolPtr = m_threadPool.lock();
        return (!poolPtr|| !poolPtr->pq_taskPriorityQueue.empty() || poolPtr->b_stopFlag);
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
    auto result = task.execute();
    cout<<result<<endl;
    b_isInLeisure = true;
    Notify();


  }
}
void ThreadPool::ThreadStruct::Stop() {
  if(m_thread.joinable()){
    m_thread.join();
  }
}
void ThreadPool::ThreadStruct::Notify() {
  /*
   *
   * 解一个锁
   *
   * */
  con_Var.notify_one();
}
