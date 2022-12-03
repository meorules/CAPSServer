#pragma once
#include <stdlib.h>
#include <iostream>

template <typename v>
class CustomVector
{
public:

  CustomVector(int size) {
    capacity = 0;
    this->size = size;
    array = new v*[size];

  }

  ~CustomVector() {
    delete array;

  }

  void push(v item) {
    if (capacity < (size * 0.7)) {
      *array[capacity] = item;
      capacity++;
    }
    else {
      resizeArray();
      *array[capacity] = item;
      capacity++;

    }
  }

  void insert(v* item, int index) {
    if (index < size) {
      array[index] = item;
    }
    else {
      std::cout << "INSERT FUNCTION BROKE";
    }
  }

  v* getItem(int index) {
    if (index <= capacity) {
      return array[index];
    }
    else return NULL;

  }

  v pop() {
    return *array[capacity];
  }

  void removeLast() {
    array[capacity] = NULL;
    capacity--;
  }

  int findItem(v item) {
    for (int i = 0; i < capacity + 1; i++) {
      if (*array[i] == item)
        return i;
    }
    return -1;
  }

  int getCapacity() {
    return capacity;
  }

  int getSize() {
    return size;
  }

private:
  v** array;
  int capacity;
  int size;

  void resizeArray() {
    v** newArray = new v*[size * 2];
    for (int i = 0; i < capacity+1; i++) {
      newArray[i] = array[i];
    }
    free(array);
    array = newArray;

    size = size * 2;

  }

};

