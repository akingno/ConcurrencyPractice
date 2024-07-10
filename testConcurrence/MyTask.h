//
// Created by jacob on 2024/7/9.
//

#ifndef TESTCONCURRENCE__MYTASK_H_
#define TESTCONCURRENCE__MYTASK_H_
#include <iostream>
#include <functional>

using TaskFunction = std::function<int(std::pair<int, int>)>;
using Range = std::pair<int,int>;

class Task{
  TaskFunction taskfunction;
  std::pair<int,int> m_range;
  int i_priority;
 public:
  Task() = default;
  Task(TaskFunction &&taskfun,Range &&range,int priority) : taskfunction(std::move(taskfun)),
                                                              m_range(range),
                                                              i_priority(priority){}

  auto execute(){
    return taskfunction(m_range);
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
