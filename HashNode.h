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
    return key;
  }

  v getValue() {
    return v;
  }

  HashNode<k, v>* getNext() {
    return nextNode;
  }

  void setNext(HashNode<k, v>* nodeToSet) {
    nextNode = nodeToSet;
  }

  std::shared_mutex lock;

private:
  k key;
  v value;

  HashNode<k,v>* nextNode;




};