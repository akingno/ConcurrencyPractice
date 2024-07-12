//
// Created by jacob on 2024/7/9.
//

#ifndef TESTCONCURRENCE__THREADPOOL_H_
#define TESTCONCURRENCE__THREADPOOL_H_
#include "SafePriorityQueue.h"
#include "SafeQueue.h"
#include "Task.h"
#include <condition_variable>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

class ThreadPool : std::enable_shared_from_this<ThreadPool>{
 public:
  static std::shared_ptr<ThreadPool> CreateThreadPool(int thread_num);

  using TaskSafePriorityQueue = SafeContainers::SafePriorityQueue<std::shared_ptr<Task<void> > >;

  ThreadPool();
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) noexcept = delete;

  void operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) noexcept = delete;

  void EnqueueTask(std::shared_ptr<Task<void>>& task);

  void EnqueueResult(long long int result);

  void WaitForAllTasksDone();


 private:

  void CreateThreads(int thread_num);
  void Run();
  void Stop();
  void ExecuteTask(std::shared_ptr<Task<void>>& task);
  bool PopTask(std::shared_ptr<Task<void>>& task);
  void CreateAccumulateThread();
  void Accumulate();

 private:
  std::vector<std::thread>                          threads_;
  std::condition_variable                           con_var_task_sig_;

  TaskSafePriorityQueue                             task_priorityQueue_;
  std::mutex                                        queue_mutex_;

  std::condition_variable                           con_var_all_tasks_done_;
  std::mutex                                        all_tasks_done_mutex_;
  std::atomic<int>                                  active_tasks_ = 0;
  std::atomic<bool>                                 has_run_flag_;

  std::atomic<bool>                                 stop_flag_;
  std::atomic<bool>                                 all_tasks_done;


  std::jthread                                      calculate_thread_;

  SafeContainers::SafeQueue<long long>              result_queue_;
  std::mutex                                        calculate_result_mutex_;
  std::condition_variable                           con_var_calculate_results_;


};
#endif//TESTCONCURRENCE__THREADPOOL_H_
