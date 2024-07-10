//
// Created by jacob on 2024/7/9.
//

#ifndef TESTCONCURRENCE__THREADPOOL_H_
#define TESTCONCURRENCE__THREADPOOL_H_
#include "MyTask.h"
#include <condition_variable>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool : std::enable_shared_from_this<ThreadPool>{


  std::vector<std::thread>                    m_threads;
  std::condition_variable                     con_Var;//wait


  std::priority_queue<Task>                   pq_taskPriorityQueue;
  std::mutex                                  mtx_queueMutex;

  bool                                        b_stopFlag;
  void Init(int);
  void Run();
  void Stop();

  public:
  explicit ThreadPool(int);
  //ThreadPool(ThreadPool&);
  ~ThreadPool();
  //void operator=(ThreadPool);
  //void operator=(ThreadPool&);
  void EnqueueTask(Task&&);

  bool IsFull() const;
};
#endif//TESTCONCURRENCE__THREADPOOL_H_
