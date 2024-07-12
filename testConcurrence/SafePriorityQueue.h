//
// Created by jacob on 2024/7/12.
//

#ifndef TESTCONCURRENCE__SAFEPRIORITYQUEUE_H_
#define TESTCONCURRENCE__SAFEPRIORITYQUEUE_H_


#include <queue>
#include <mutex>
#include <condition_variable>


namespace SafeContainers {

template<typename T>
class SafePriorityQueue {
 public:
  SafePriorityQueue() = default;

  /*
   *
   * Because the existence of mutex and condition variables, copy and move is not allowed
   *
   * */
  SafePriorityQueue(const SafePriorityQueue &) = delete;
  SafePriorityQueue &operator=(const SafePriorityQueue &) = delete;
  SafePriorityQueue(SafePriorityQueue &&) = delete;
  SafePriorityQueue &operator=(SafePriorityQueue &&) = delete;

  ~SafePriorityQueue() = default;

  /*
   *
   * Push elems in queue
   *
   * */
  void push(const T &value) {
    std::lock_guard<std::mutex> lock(mutex_);
    priority_queue_.push(value);
  }

  /*
   *
   * Pop elems from queue
   *
   * */
  void pop(T &value) {
    std::unique_lock<std::mutex> lock(mutex_);
    value = priority_queue_.top();
    priority_queue_.pop();
  }

  /*
   *
   * Check if queue is empty
   *
   * */
  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return priority_queue_.empty();
  }

  /*
   *
   * Return the size of the queue
   *
   * */
  std::size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return priority_queue_.size();
  }

  /*
   *
   * Get the top element of the queue but not remove it
   *
   * */
  bool top(T &value) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (priority_queue_.empty()) {
      return false;
    }
    value = priority_queue_.top();
    return true;
  }

 private:
  std::priority_queue<T> priority_queue_;//the priority queue
  mutable std::mutex mutex_;
};

}


#endif//TESTCONCURRENCE__SAFEPRIORITYQUEUE_H_
