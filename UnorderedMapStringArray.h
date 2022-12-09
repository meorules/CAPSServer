#pragma once
#include "DataStructureAPI.h"
#include <unordered_map>
#include <vector>

class UnorderedMapStringArray :
  public DataStructureAPI
{
public:
  UnorderedMapStringArray();
  ~UnorderedMapStringArray();

  void PostFunction(string topic, string message, int id);
  string findReadTopic();
  int findReadMessage(string topic);

  string ListFunction();
  int CountFunction(string topic);
  string ReadFunction(string topic, int messagedID);
  bool TopicExists(string topic);


private:
  unordered_map<string, string*>* dataStructure;
  bool structNotEmpty();
  vector<string>* keyList;

  static const unsigned int ARRAY_SIZE = 100;

};

