#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

template<typename T>
class threadsave_queue{
private:
  mutable std::mutex mut;//必须是mutable，因为empty是const方法，但是要锁mut，锁操作就是改变操作
  std::queue<T> data_queue;
  std::condition_variable data_cond;
public:
  threadsave_queue(){}
  threadsave_queue(threadsave_queue const& other){
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue = other.data_queue();
  }
  void push(T new_value){
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  }
  void wait_and_pop(T& value){
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this]{return !data_queue.empty();});
    value = data_queue.front();
    data_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop(){
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this]{return !data_queue.empty();});
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool empty()const{
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};
