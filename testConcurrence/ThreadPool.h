//
// Created by jacob on 2024/7/9.
//

#ifndef TESTCONCURRENCE__THREADPOOL_H_
#define TESTCONCURRENCE__THREADPOOL_H_
#include "Task.h"
#include <condition_variable>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool : std::enable_shared_from_this<ThreadPool>{


  std::vector<std::thread>                          m_threads;
  std::condition_variable                           con_Var;//wait


  std::priority_queue<std::shared_ptr<Task<void>>>  pq_taskPriorityQueue;
  std::mutex                                        mtx_queueMutex;

  std::condition_variable                           all_tasks_done;
  std::mutex                                        all_tasks_done_mutex;
  std::atomic<int>                                  active_tasks = 0;
  std::atomic<bool>                                 b_hasRun;

  bool                                              b_stopFlag;
  void Init(int);
  void Run();
  void Stop();


  public:
   ThreadPool() = delete;
   explicit ThreadPool(int);
   ~ThreadPool();

   ThreadPool(const ThreadPool&) = delete;
   ThreadPool(ThreadPool&&) noexcept = delete;

   void operator=(const ThreadPool&) = delete;
   ThreadPool& operator=(ThreadPool&&) noexcept = delete;

   void EnqueueTask(std::shared_ptr<Task<void>>& );
   void WaitForAllTasksDone();

};
#endif//TESTCONCURRENCE__THREADPOOL_H_
