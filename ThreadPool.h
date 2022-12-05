#pragma once
#include <mutex>
#include <functional>
#include <queue>
#include <thread>
#include <functional>
#include <stdexcept>
#include <future>
#include <utility>

class ThreadPool
{
public: 
  
  ThreadPool(int numberOfThreads= std::thread::hardware_concurrency());
  ~ThreadPool(){}
  void QueueJob(const std::function<void()>& job);

  //template<class F, class... Args>
  //inline void enqueue(F&& f, Args&&... args);

  void Stop();

private:
  void ThreadLoop();

  bool terminate = false;
  int numberOfThreads;
  std::mutex queue_mutex;
  std::condition_variable cvJobQueue;
  std::queue<std::function<void()>> jobQueue;
  std::vector<std::thread> threads;

};

inline ThreadPool::ThreadPool(int numberOfThreads) {

  
  this-> numberOfThreads = numberOfThreads;
  threads.resize(numberOfThreads);
  for (int i = 0; i < numberOfThreads; i++) {
    threads.at(i) = std::thread(&ThreadPool::ThreadLoop);
  }
}

inline void ThreadPool::ThreadLoop() {
  std::function<void()> job;

  {std::unique_lock <std::mutex> lock(queue_mutex);
  while (jobQueue.size() < 1) {
    cvJobQueue.wait(lock);
  }

  if (jobQueue.size() > 1) {
    job = jobQueue.front();
    jobQueue.pop();
  }
  else if (terminate) {
    return;
  }
  job();
  }

}

inline void ThreadPool::QueueJob(const std::function<void()>& job) {
  if (!terminate) {
    std::unique_lock <std::mutex> lock(queue_mutex);
    jobQueue.push(job);
    cvJobQueue.notify_one();
  }
}

//template<class F, class... Args>
//inline void ThreadPool::enqueue(F&& f, Args&&... args) {
//
//  auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//
//  if (!terminate) {
//    std::unique_lock <std::mutex> lock(queue_mutex);
//    
//    jobQueue.emplace(task);
//    cvJobQueue.notify_one();
//  }
//
//
//}

inline void ThreadPool::Stop() {
  {
    std::unique_lock <std::mutex> lock(queue_mutex);
    terminate = true;
  }
  cvJobQueue.notify_all();

  for (int i = 0; i < numberOfThreads; i++) {
    threads.at(i).join();
  }


}

