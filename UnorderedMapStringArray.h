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

  const string ListFunction();
  const int CountFunction(string topic);
  const string ReadFunction(string topic, int messagedID);
  const bool TopicExists(string topic);


private:
  unordered_map<string, string*>* dataStructure;
  const bool structNotEmpty();
  vector<string>* keyList;

  static const unsigned int ARRAY_SIZE = 100;

};

