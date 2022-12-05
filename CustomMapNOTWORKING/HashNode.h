#pragma once
#include <shared_mutex>


template<typename v>
class HashNode
{
public:

  HashNode(string key) {
    this->key = key;
    value = new v();
    nextNode = nullptr;
  }

  HashNode(string keyToSet, v valueToSet) {
    key = keyToSet;
    value = valueToSet;
    nextNode = nullptr;
  }

  ~HashNode() {
    delete key;
    delete value;
    delete nextNode;
  }

  string getKey() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return key;
  }

  v* getValue() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return value;
  }

  HashNode<v>* getNext() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return nextNode;
  }

  void setNext(HashNode<v>* nodeToSet) {
    std::unique_lock<std::shared_mutex> mutex(lock);
    nextNode = nodeToSet;
  }

  std::shared_mutex lock;

private:
  string key;
  v* value;

  HashNode<v>* nextNode;




};