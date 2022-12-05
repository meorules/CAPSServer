#pragma once
#include "DataStructureAPI.h"
#include <unordered_map>
#include "topicLock.h"
#include <vector>

class topicLockMap :
    public DataStructureAPI
{
public:
  topicLockMap();
  ~topicLockMap();

  int PostFunction(string topic, string message);
  string ListFunction();
  int CountFunction(string topic);
  string ReadFunction(string topic, int messagedID);
  


private:
  unordered_map<string, topicLock*>* dataStructure;
  bool structNotEmpty();
  bool TopicExists(string topic);


};

inline topicLockMap::topicLockMap() {
  dataStructure = new unordered_map<string, topicLock*>;

}
inline topicLockMap::~topicLockMap() {
  delete dataStructure;
}


/**
 * @param Topic is the topic for which the post needs to take place, topics are created if they dont exist.
 * message is the message added the topic specified.
 * @return an int value corresponding to the id of the message which was saved
 */
inline int topicLockMap::PostFunction(string topic, string message)
{
  int messageId;
  if (TopicExists(topic)) {
    shared_lock<shared_mutex> mutex(lock);
    topicLock* topicArray = dataStructure->at(topic);
     messageId = topicArray->push(message);
     return messageId;
  }
  else {
    topicLock* topicArray = new topicLock(100);
    messageId = topicArray->push(message);
    {
      unique_lock<shared_mutex> mutex(lock);
      dataStructure->insert({ topic, topicArray });
    }
    return messageId;
  }
}


/**
 * @return a string containing the topics list seperated by @ and #
 */
inline string topicLockMap::ListFunction()
{
  string topicList = "";
  if (structNotEmpty()) {
    {
      shared_lock<shared_mutex> mutex(lock);
      unordered_map<string, topicLock*>::iterator it = dataStructure->begin();
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
inline int topicLockMap::CountFunction(string topic)
{
  int messageCount = 0;
  
  if (TopicExists(topic)) {
    shared_lock<shared_mutex> mutex(lock);
    messageCount = dataStructure->at(topic)->getCapacity();
  }
  return messageCount;

}


/**
 * @param Topic corresponding to the post, the messageID for the int id where the message is saved
 * @return the string value of the message, if no message is found, a blank string will be returned
 */
inline string topicLockMap::ReadFunction(string topic, int messagedID)
{
  string message="";
  
  if (TopicExists(topic)) {
    shared_lock<shared_mutex> mutex(lock);
    if (dataStructure->at(topic)->getCapacity() > messagedID) {
      message = dataStructure->at(topic)->getMessage(messagedID);
    }
  }
  return message;
}

/**
 * @param Topic to be searched for
 * @return bool value, True if the topic is found, False if the topic does not exist
 */
inline bool topicLockMap::TopicExists(string topic) {
  {
    if (structNotEmpty()) {
      shared_lock<shared_mutex> mutex(lock);
      return dataStructure->contains(topic);
    }
    else {
      return false;
    }
  }
}

inline bool topicLockMap::structNotEmpty() {
  {
    shared_lock<shared_mutex> mutex(lock);
    return !dataStructure->empty();
  }
}