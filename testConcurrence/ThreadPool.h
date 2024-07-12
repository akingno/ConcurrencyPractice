//
// Created by jacob on 2024/7/9.
//

#ifndef TESTCONCURRENCE__THREADPOOL_H_
#define TESTCONCURRENCE__THREADPOOL_H_
#include "Task.h"
#include <condition_variable>
#include <iostream>
#include <memory>
#include "SafePriorityQueue.h"
#include <thread>
#include <vector>

class ThreadPool : std::enable_shared_from_this<ThreadPool>{
 public:

  using TaskSafePriorityQueue = SafeContainers::SafePriorityQueue<std::shared_ptr<Task<void>>>;

  ThreadPool() = delete;
  explicit ThreadPool(int thread_num);
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) noexcept = delete;

  void operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) noexcept = delete;

  void EnqueueTask(std::shared_ptr<Task<void>>& task);
  void WaitForAllTasksDone();


 private:

  void Init(int thread_num);
  void Run();
  void Stop();
  void ExecuteTask(std::shared_ptr<Task<void>>& task);
  bool PopTask(std::shared_ptr<Task<void>>& task);

  std::vector<std::thread>                          threads_;
  std::condition_variable                           con_var_task_sig_;

  TaskSafePriorityQueue                             task_priorityQueue_;
  std::mutex                                        queue_mutex_;

  std::condition_variable                           con_var_all_tasks_done_;
  std::mutex                                        all_tasks_done_mutex_;
  std::atomic<int>                                  active_tasks_ = 0;
  std::atomic<bool>                                 has_run_flag_;

  std::atomic<bool>                                 stop_flag_;


};
#endif//TESTCONCURRENCE__THREADPOOL_H_
