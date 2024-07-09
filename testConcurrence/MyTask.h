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
  int priority;
 public:
  Task() = default;
  Task(TaskFunction &taskfun,Range &range) : taskfunction(std::move(taskfun)),
                                              m_range(range),
                                              priority(0){}

  void execute(){
    taskfunction(m_range);
  }
  bool operator<(const Task& other) const{
    return priority < other.priority;
  }
  bool operator==(const Task& other) const{
    return priority == other.priority;
  }
  int getPriority() const {
    return priority;
  }
};



#endif//TESTCONCURRENCE__MYTASK_H_
