#pragma once
#include <shared_mutex>


template<typename k,typename v>
class HashNode
{
public:

  HashNode() {
    key = NULL;
    value = NULL;
    nextNode = nullptr;
  }

  HashNode(k keyToSet, v valueToSet) {
    key = keyToSet;
    value = valueToSet;
    nextNode = nullptr;
  }

  ~HashNode() {
    delete key;
    delete value;
    delete nextNode;
  }

  k getKey() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return key;
  }

  v getValue() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return value;
  }

  HashNode<k, v>* getNext() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return nextNode;
  }

  void setNext(HashNode<k, v>* nodeToSet) {
    std::unique_lock<std::shared_mutex> mutex(lock);
    nextNode = nodeToSet;
  }

  std::shared_mutex lock;

private:
  k key;
  v value;

  HashNode<k,v>* nextNode;




};