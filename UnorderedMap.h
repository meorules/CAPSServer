#pragma once
#include "DataStructureAPI.h"
#include <unordered_map>
#include <vector>

class UnorderedMap :
    public DataStructureAPI
{
public:
  UnorderedMap();
  ~UnorderedMap();

  int PostFunction(string topic, string message);
  string ListFunction();
  int CountFunction(string topic);
  string ReadFunction(string topic, int messagedID);
  bool TopicExists(string topic);


private:
  unordered_map<string, vector<string>*>* dataStructure;
  bool structNotEmpty();

};

inline UnorderedMap::UnorderedMap() {
  dataStructure = new unordered_map<string, vector<string>*>;

}
inline UnorderedMap::~UnorderedMap() {
  delete dataStructure;
}


/**
 * @param Topic is the topic for which the post needs to take place, topics are created if they dont exist.
 * message is the message added the topic specified.
 * @return an int value corresponding to the id of the message which was saved
 */
inline int UnorderedMap::PostFunction(string topic, string message)
{
  if (structNotEmpty){
    vector<string>* topicArray;
    if (TopicExists(topic)) {
      unique_lock(lock);
      topicArray = dataStructure->at(topic);
      topicArray->push_back(message);
    }
    else {
      unique_lock(lock);
      topicArray = new vector<string>;
      topicArray->push_back(message);

      dataStructure->insert({ topic, topicArray });
    }
    return topicArray->size() - 1;
  }

}


/**
 * @return a string containing the topics list seperated by @ and #
 */
inline string UnorderedMap::ListFunction()
{
  string topicList = "";
  if (structNotEmpty) {
    {
      shared_lock(lock);
      unordered_map<string, vector<string>*>::iterator it = dataStructure->begin();
      while (it != dataStructure->end()) {
        topicList.append("@" + it->first + "#");
        it++;
      }
    }
  }
  return topicList;

}


/**
 * @param Takes a topic to check
 * @return an int value corresponding to the number of messages for the topic provided,
 * returns 0 if topic does not exist
 */
inline int UnorderedMap::CountFunction(string topic)
{
  int messageCount = 0;
  if (structNotEmpty) {
    if (TopicExists(topic)) {
      shared_lock(lock);
      messageCount = dataStructure->at(topic)->size();
    }
  }
  return messageCount;

}


/**
 * @param Topic corresponding to the post, the messageID for the int id where the message is saved
 * @return the string value of the message, if no message is found, a blank string will be returned
 */
inline string UnorderedMap::ReadFunction(string topic, int messagedID)
{
  string message="";
  if (structNotEmpty) {
    if (TopicExists(topic)) {
      shared_lock(lock);
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
inline bool UnorderedMap::TopicExists(string topic) {
  {
    shared_lock(lock);
    return dataStructure->contains(topic);
  }
}

inline bool UnorderedMap::structNotEmpty() {
  {
    shared_lock(lock);
    return !dataStructure->empty();
  }
}