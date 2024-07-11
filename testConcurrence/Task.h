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
  std::function<ResultType()> f_func;
  int i_priority;
 public:

  Task() = delete;
  Task(int priority) = delete;
  Task(std::function<ResultType()> f, int priority) : i_priority(priority), f_func(std::move(f)) {}
  Task(std::function<ResultType()> &&f, int priority) = delete;

  Task(const Task&) = delete;
  Task(Task && t) noexcept{
    f_func = std::move(t.f_func);
    i_priority = t.i_priority;
  }



  template<typename... Args>
  auto execute(Args&&... args){
    return f_func(args...);
  }

  void operator=(const Task&) = delete;
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