//
// Created by jacob on 2024/7/9.
//

#ifndef TESTCONCURRENCE__MYTASK_H_
#define TESTCONCURRENCE__MYTASK_H_
#include <atomic>
#include <functional>
#include <iostream>

using TaskFunction = std::function<int(std::pair<int, int>,std::atomic<int>&)>;
using Range = std::pair<int,int>;

class Task{
  TaskFunction taskfunction;
  std::pair<int,int> m_range;
  int i_priority;
  std::atomic<int>& m_totalSum;
 public:
  Task() = delete;
  Task(const TaskFunction taskfun,Range &&range,int priority, std::atomic<int>& totalSum) :taskfunction(taskfun),
                                                                                        m_range(range),
                                                                                        i_priority(priority),
                                                                                        m_totalSum(totalSum){}

  auto execute(){
    return taskfunction(m_range,m_totalSum);
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



#endif//TESTCONCURRENCE__MYTASK_H_
