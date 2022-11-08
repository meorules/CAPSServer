#pragma once
#include <mutex>
#include <functional>
#include <queue>
#include <thread>


class ThreadPool
{
public: 
  
  ThreadPool(int numberOfThreads= std::thread::hardware_concurrency());
  ~ThreadPool(){}
  void QueueJob(const std::function<void()>& job);
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

inline ThreadPool::ThreadPool(int numberOfThreads = std::thread::hardware_concurrency()) {
  this-> numberOfThreads = numberOfThreads;
  threads.resize(numberOfThreads);
  for (int i = 0; i < numberOfThreads; i++) {
    threads.at(i) = std::thread(&ThreadPool::ThreadLoop);
  }
}

inline void ThreadPool::ThreadLoop() {
  std::unique_lock <std::mutex> lock(queue_mutex);
  while (jobQueue.size() < 1) {
    cvJobQueue.wait(lock);
  }
  
  if (jobQueue.size() > 1) {
    std::function<void()> job;
    job = jobQueue.front();
    jobQueue.pop();
    job();
  }
  else if (terminate) {
    return;
  }
}

inline void ThreadPool::QueueJob(const std::function<void()>& job) {
  if (!terminate) {
    std::unique_lock <std::mutex> lock(queue_mutex);
    jobQueue.push(job);
    cvJobQueue.notify_one();
  }
}

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

