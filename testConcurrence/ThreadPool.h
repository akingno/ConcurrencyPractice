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

  class ThreadStruct{
    std::thread                 m_thread;
    std::atomic<bool>           b_isInLeisure;
    std::weak_ptr<ThreadPool>   m_threadPool;
    std::mutex                  m_condition_mutex;
    std::condition_variable     con_Var;//wait
    friend class ThreadPool;
   public:
    explicit ThreadStruct(std::shared_ptr<ThreadPool>);
    ~ThreadStruct();
    void Run();
    void Stop();
    void Notify();
  };

  std::vector<std::shared_ptr<ThreadStruct>>  m_threads;
  std::priority_queue<Task>                   pq_taskPriorityQueue;
  std::mutex                                  mtx_queueMutex;

  bool                                        b_stopFlag;


  void Init();
  bool IsFull() const;
 public:
  explicit ThreadPool();
  //ThreadPool(ThreadPool&);
  ~ThreadPool();
  //void operator=(ThreadPool);
  //void operator=(ThreadPool&);
  void AddingThreads(int);
  void NotifiedByTaskqueue();
  void DispatchTask();
  void Stop();
  void EnqueueTask(Task);






};
#endif//TESTCONCURRENCE__THREADPOOL_H_
