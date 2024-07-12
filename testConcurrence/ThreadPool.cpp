//
// Created by jacob on 2024/7/9.
//

#include "ThreadPool.h"
#include <mutex>
using namespace std;
ThreadPool::ThreadPool() : stop_flag_(false), has_run_flag_(false){

}
ThreadPool::~ThreadPool() {
  /*
   *
   * 停止：删除所有thread
   *
   * */
  Stop();
}


void ThreadPool::EnqueueResult(long long result){
  result_queue_.enqueue(result);
  con_var_calculate_results_.notify_all();
}

std::shared_ptr<ThreadPool> ThreadPool::CreateThreadPool(int thread_num) {
  auto ptr = make_shared<ThreadPool>();
  ptr->CreateThreads(thread_num);
  ptr->CreateAccumulateThread();

  return ptr;

}
void ThreadPool::CreateAccumulateThread(){
  calculate_thread_ = jthread(&ThreadPool::Accumulate,this);


}

void ThreadPool::Accumulate() {
  cout<<"Accumulate thread begin"<<endl;
  long long sum = 0;
  do{
    long long value = 0;
    while (result_queue_.dequeue(value)) {
      //cout<<value<<","<<endl;
      sum += value;
    }
    //cout<<"add end:"<<sum<<endl;
  }while(!all_tasks_done.load());

  cout<<"Sum By Accumulator:"<<sum<<endl;
}

void ThreadPool::Stop() {
  /*
   *
   * 所有thread，能join都join
   * b_stopFlag = true
   *
   * */

  stop_flag_ = true;

  con_var_task_sig_.notify_all();

  if(threads_.empty()){
    return;
  }

  for(auto& thd: threads_){
    if(thd.joinable()){
      thd.join();
    }
  }


  threads_.clear();
  //cout<<"clean over"<<endl;


}

void ThreadPool::EnqueueTask(shared_ptr<Task<void>>& task) {
  /*
   *
   * 1. 将task放入taskqueue
   * 2. 用con_var通知一个线程
   *
   * */

  task_priorityQueue_.push(task);
  active_tasks_.fetch_add(1);
  //cout<<"taskPQ size:"<<pq_taskPriorityQueue.size()<<endl;

  con_var_task_sig_.notify_one();

}



void ThreadPool::CreateThreads(int num_threads) {
  /*
   * 初始化，Adding threads
   *
   * */
  for(int i = 0; i < num_threads; ++i){
    threads_.emplace_back(&ThreadPool::Run,this);
  }

  //AddingThreads(num_threads);
}


void ThreadPool::Run() {
  while(!stop_flag_){

    shared_ptr<Task<void>> task;
    {
      unique_lock<mutex> lock(queue_mutex_);
      /*
       *
       * 如果taskqueue有任务或者已经结束了就解锁
       *
       * */
      con_var_task_sig_.wait(lock, [this]() {
        return (!task_priorityQueue_.empty() || stop_flag_.load());
      });
    }

    /*
     *
     * 如果已经结束了则返回
     *
     * */
    if(stop_flag_.load()){
      return;
    }

    /*
     *
     * 从safe task PQ中取task，没取到继续下个循环，然后卡住
     *
     * */
    if(!PopTask(task)){
      continue;
    }

    ExecuteTask(task);

    /*
     *
     * 执行完，如果此时没有任何task执行，通知main进入下一步
     *
     * */
    if (active_tasks_.load() == 0) {
      cout<<"all task done"<<endl;
      std::lock_guard<std::mutex> lock(all_tasks_done_mutex_);
      con_var_all_tasks_done_.notify_one();
      all_tasks_done = true;
    }

  }
}


void ThreadPool::WaitForAllTasksDone() {
  /*
   *
   * main在新建完task后会开始执行这个函数
   * 这个函数会一直等待，直到所有tasks都执行完，再进行下一步
   *
   * */
  unique_lock<mutex> lock(all_tasks_done_mutex_);
  con_var_all_tasks_done_.wait(lock,[this](){
    return (active_tasks_.load() == 0 && has_run_flag_.load());
  });
  stop_flag_ = true;



}

void ThreadPool::ExecuteTask(shared_ptr<Task<void>>& task) {
  /*
     *
     * task执行
     * 此处可以带参数，本例不用
     *
     *
     * */
  (*task)();
  has_run_flag_ = true;
  active_tasks_.fetch_sub(1);
}


bool ThreadPool::PopTask(shared_ptr<Task<void>>& task) {
  /*
       *
       * 如果taskqueue有task则取出执行
       *
       * */
  if (!task_priorityQueue_.empty()) {
    task_priorityQueue_.pop(task);
    return true;
  } else {
    return false;
  }
}

