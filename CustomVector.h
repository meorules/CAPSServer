#pragma once
#include <stdlib.h>
#include <iostream>
#include <shared_mutex>

template <typename v>
class CustomVector
{
public:

  CustomVector(int size=10000) {
    capacity = 0;
    this->size = size;
    array = new v*[size];

  }

  ~CustomVector() {
    delete array;

  }

  void push(v item) {
    if (capacity < (size * 0.7)) {
      std::unique_lock<std::shared_mutex> mutex(lock);
      *array[capacity] = item;
      capacity++;
    }
    else {
      resizeArray();
      std::unique_lock<std::shared_mutex> mutex(lock);
      *array[capacity] = item;
      capacity++;

    }
  }

  void insert(v* item, int index) {
    if (index < size) {
      std::unique_lock<std::shared_mutex> mutex(lock);
      array[index] = item;
      capacity++;
    }
    else {
      std::cout << "INSERT FUNCTION BROKE";
    }
  }

  v* getItem(int index) {
    if (index <= capacity) {
      std::shared_lock<std::shared_mutex> mutex(lock);
      return array[index];
    }
    else return NULL;

  }

  v pop() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return *array[capacity];
  }

  void removeLast() {
    std::unique_lock<std::shared_mutex> mutex(lock);
    array[capacity] = NULL;
    capacity--;
  }

  int findItem(v item) {
    for (int i = 0; i < capacity + 1; i++) {
      std::shared_lock<std::shared_mutex> mutex(lock);
      if (*array[i] == item)
        return i;
    }
    return -1;
  }

  int getCapacity() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return capacity;
  }

  int getSize() {
    std::shared_lock<std::shared_mutex> mutex(lock);
    return size;
  }

private:
  v** array;
  int capacity;
  int size;
  std::shared_mutex lock;

  void resizeArray() {
    std::unique_lock<std::shared_mutex> mutex(lock);
    v** newArray = new v*[size * 2];
    for (int i = 0; i < capacity+1; i++) {
      newArray[i] = array[i];
    }
    free(array);
    array = newArray;

    size = size * 2;

  }

};

