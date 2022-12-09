#pragma once
#include "DataStructureAPI.h"
#include <unordered_map>
#include <vector>

class NoLockUnorderedMap :
    public DataStructureAPI
{
public:
  NoLockUnorderedMap();
  ~NoLockUnorderedMap();

  const int PostFunction(string topic, string message);
  const string ListFunction();
  const int CountFunction(string topic);
  const string ReadFunction(string topic, int messagedID);
  const bool TopicExists(string topic);


private:
  unordered_map<string, vector<string>*>* dataStructure;
  const bool structNotEmpty();


};

inline NoLockUnorderedMap::NoLockUnorderedMap() {
  dataStructure = new unordered_map<string, vector<string>*>;

}
inline NoLockUnorderedMap::~NoLockUnorderedMap() {
  delete dataStructure;
}


/**
 * @param Topic is the topic for which the post needs to take place, topics are created if they dont exist.
 * message is the message added the topic specified.
 * @return an int value corresponding to the id of the message which was saved
 */
inline const int NoLockUnorderedMap::PostFunction(string topic, string message)
{
  vector<string>* topicArray;
  if (structNotEmpty()) {
    if (TopicExists(topic)) {
      topicArray = dataStructure->at(topic);
      topicArray->push_back(message);
      return topicArray->size() - 1;
    }
    else {
      topicArray = new vector<string>;
      topicArray->push_back(message);

      dataStructure->emplace(topic, topicArray);
      return topicArray->size() - 1;
    }
  }
  else {
    topicArray = new vector<string>;
    topicArray->push_back(message);

    dataStructure->emplace(topic, topicArray);
    return topicArray->size() - 1;

  }

  
}


/**
 * @return a string containing the topics list seperated by @ and #
 */
inline const string NoLockUnorderedMap::ListFunction()
{
  string topicList = "";
  if (structNotEmpty()) {
    {
      unordered_map<string, vector<string>*>::iterator it = dataStructure->begin();
      while (it != dataStructure->end()) {
        topicList.append(it->first + "#");
        it++;
      }
    }
    if (topicList != "") {
      topicList.pop_back();
    }
  }
  return topicList;

}


/**
 * @param Takes a topic to check
 * @return an int value corresponding to the number of messages for the topic provided,
 * returns 0 if topic does not exist
 */
inline const int NoLockUnorderedMap::CountFunction(string topic)
{
  int messageCount = 0;
  if (structNotEmpty()) {
    if (TopicExists(topic)) {
      messageCount = dataStructure->at(topic)->size();
    }
  }
  return messageCount;

}


/**
 * @param Topic corresponding to the post, the messageID for the int id where the message is saved
 * @return the string value of the message, if no message is found, a blank string will be returned
 */
inline const string NoLockUnorderedMap::ReadFunction(string topic, int messagedID)
{
  string message="";
  if (structNotEmpty()) {
    if (TopicExists(topic)) {
      if (dataStructure->at(topic)->size() > messagedID) {
        message = dataStructure->at(topic)->at(messagedID);
      }
    }
  }
  return message;
}

/**
 * @param Topic to be searched for
 * @return bool value, True if the topic is found, False if the topic does not exist
 */
inline const bool NoLockUnorderedMap::TopicExists(string topic) {
  {
    return dataStructure->contains(topic);
  }
}

inline const bool NoLockUnorderedMap::structNotEmpty() {
  {
    return !dataStructure->empty();
  }
}