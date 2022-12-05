#pragma once
#include <shared_mutex>
#include <vector>

using namespace std;

class topicLock
{
public:
  topicLock(int size);
  ~topicLock();

  int push(string message); 
  //void insert(string message, int id); DEPRICATED FUNCTION (BREAKS CAPACITY)
  string getMessage(int index);

  int getCapacity();

  int getSize();

private:
  std::shared_mutex lock;
  int capacity;
  int size;
  vector<string>* messageArray;

  //void resizeArray();

};

inline topicLock::topicLock(int size) : capacity(0),size(size) {
  unique_lock<shared_mutex> mutex(lock);
  messageArray = new vector<string>;
}

inline topicLock::~topicLock() {
  unique_lock<shared_mutex> mutex(lock);
  delete messageArray;
}

inline int topicLock::push(string message)
{
  /*const int reSizePercentage = size * 0.7;
  if (capacity >= reSizePercentage) {
    resizeArray();
  }*/
  unique_lock<shared_mutex> mutex(lock);
  messageArray->push_back(message);
  capacity++;
  return capacity - 1;
}

//inline void topicLock::insert(string message, int id)
//{
//  if (id < size) {
//    const int reSizePercentage = size * 0.7;
//    if (capacity >= reSizePercentage) {
//      resizeArray();
//    }
//    unique_lock<shared_mutex> mutex(lock);
//    if (messageArray[id] == "") {
//      messageArray[id] = message;
//    }
//  }
//}


inline string topicLock::getMessage(int index)
{
  if (index < capacity) {
    shared_lock<shared_mutex> mutex(lock);
    return messageArray->at(index);
  }
  else {
    return "";
  }
}




int topicLock::getCapacity() {
  shared_lock<shared_mutex> mutex(lock);
  return capacity;
}

int topicLock::getSize() {
  shared_lock<shared_mutex> mutex(lock);
  return size;
}

//void topicLock::resizeArray() {
//  std::unique_lock<std::shared_mutex> mutex(lock);
//  string* newArray[] = new string[size * 2];
//  for (int i = 0; i < capacity; i++) {
//    newArray[i] = *messageArray[i];
//  }
//  messageArray = newArray;
//  delete newArray;
//
//  size = size * 2;
//
//}