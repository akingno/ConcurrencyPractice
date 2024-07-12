//
// Created by jacob on 2024/7/11.
//

#ifndef TESTCONCURRENCE__TASK_H_
#define TESTCONCURRENCE__TASK_H_

#include <atomic>
#include <functional>
#include <iostream>



template<typename ResultType>
class Task{
  std::function<ResultType()>     f_func;
  int                             i_priority; // used for priority queue
 public:

  Task() = delete;
  Task(int priority) = delete;
  Task(std::function<ResultType()> f, int priority) : i_priority(priority), f_func(std::move(f)) {}

  /*
   *
   * There may be things in f that can not be moved, for example atomic<>
   * Thus constructor using std::move is not allowed
   *
   * */
  Task(std::function<ResultType()> &&f, int priority) = delete;
  Task(const Task&) = delete;
  Task(Task && t) = delete;

  void operator=(const Task&) = delete;

  void operator()(){
    f_func();
  }

  Task& operator=(Task&& t) noexcept{
    f_func = std::move(t.f_func);
    i_priority = t.i_priority;
    return *this;
  }


  bool operator<(const Task& other) const{
    return i_priority < other.i_priority;
  }
  bool operator==(const Task& other) const{
    return i_priority == other.i_priority;
  }


  int getPriority() const {
    return i_priority;
  }

};










#endif//TESTCONCURRENCE__TASK_H_
