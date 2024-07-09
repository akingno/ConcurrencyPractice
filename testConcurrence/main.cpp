#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <queue>
#include <functional>

const int NUM_THREADS = 4;
using namespace std;
using Task = function<int(std::pair<int, int>)>;

/*
 * 封装Task，令其可以比较
 *
 * */
class TaskWithPriority{
    Task task;
    int priority;

    bool operator<(const TaskWithPriority& other) const{
      return priority < other.priority;
    }
};





auto SplitRange(const int low,const int high) {
  /*
   * 切割成 2 * threads 份
   * */
  int subRangeLength = ((high - low) + 1) / (2 * NUM_THREADS);
  auto subRange = make_shared<vector<pair<int,int>>>();
  for(int i = low; i < high;){
    if(i + subRangeLength <= high){
      subRange->emplace_back(i,i+subRangeLength-1);
      i += subRangeLength;
    }
    else{
      subRange->emplace_back(i,high);
      break;
    }
  }
  for(const auto & iter : *subRange){
    cout<<iter.first<<","<<iter.second<<endl;
  }
  return subRange;
}

/*
 *
 * 判断是否是素数
 *
 * */
bool Is_prime(int n) {
  if (n <= 1) return false;
  for (int i = 2; i * i <= n; ++i) {
    if (n % i == 0) return false;
  }
  return true;
}

/*
 *
 * 输入求素数范围
 *
 * */
tuple<int, int> LoadInput(){
  int low,high;
  cout << "Please enter two numbers" << endl;
  cin >> low >> high;
  return make_tuple(low,high);
}

/*
 * 求两个数中间的素数之和
 *
 * */
int main() {
  auto [low, high] = LoadInput();

  /*
   * subRange中，[lowest, highest]
   *
   * */
  auto subRange = SplitRange(low, high);
  vector<int> primes;

  priority_queue<TaskWithPriority> taskPriorityQueue;






  return 0;
}


