#pragma once
#include "DataStructureAPI.h"

#include "CustomVector.h"
#include "HashNode.h"


class CustomMap :
    public DataStructureAPI
{
public:
  CustomMap();
  ~CustomMap();

  int PostFunction(string topic, string message);
  string ListFunction();
  int CountFunction(string topic);
  string ReadFunction(string topic, int messagedID);
  bool TopicExists(string topic);


private:
  CustomVector<HashNode<char*,CustomVector<char*>>>* vec;
  unsigned int hash(char* str);

  bool structNotEmpty();

};

inline CustomMap::CustomMap() {
  vec = new CustomVector<HashNode<char*, CustomVector<char*>>>(100000);

}
inline CustomMap::~CustomMap() {
  delete vec;
}


/**
 * @param Topic is the topic for which the post needs to take place, topics are created if they dont exist.
 * message is the message added the topic specified.
 * @return an int value corresponding to the id of the message which was saved
 */
inline int CustomMap::PostFunction(string topic, string message)
{

  char* topicChars = new char[topic.length() + 1];
  strcpy(topicChars, topic.c_str());
  char* messageChars = new char[topic.length() + 1];
  strcpy(messageChars, topic.c_str());

  HashNode<char*, CustomVector<char*>>* topicArray;


  if (structNotEmpty()) {
    if (TopicExists(topic)) {
      int index = hash(topicChars);
      {
        std::shared_lock<std::shared_mutex> mutex(lock);
        topicArray = vec->getItem(index);

      }
      {
        std::unique_lock<std::shared_mutex> mutex(topicArray->lock);
        while (topicArray->getKey() != topicChars || topicArray->getNext() != nullptr) {
          topicArray = topicArray->getNext();
        }
        if (topicArray->getKey() == topicChars) {
          topicArray->getValue().push(messageChars);
          delete[] topicChars;
          delete[] messageChars;
          return topicArray->getValue().getCapacity() - 1;
        }
      }
    }
    else {
      CustomVector<char*> topicMessages = CustomVector<char*>(100);
      topicMessages.push(messageChars);
      HashNode<char*, CustomVector<char*>>* topicArray = new HashNode<char*, CustomVector<char*>>(topicChars, topicMessages);
      int index = hash(topicChars);
      HashNode<char*, CustomVector<char*>>* currentArray = vec->getItem(index);
      {
        std::unique_lock<std::shared_mutex> mutex(lock);
        if (currentArray == NULL) {
          vec->insert(topicArray, index);
          delete[] topicChars;
          delete[] messageChars;
        }
        else {
          while (currentArray->getNext() != nullptr) {
            currentArray = currentArray->getNext();
          }
          currentArray->setNext(topicArray);
          delete[] topicChars;
          delete[] messageChars;
        }
        return topicMessages.getCapacity() - 1;
      }
    }

  }

}


/**
 * @return a string containing the topics list seperated by @ and #
 */
inline string CustomMap::ListFunction()
{
  /*string topicList = "";
  if (structNotEmpty()) {
    {
      shared_lock<shared_mutex> mutex(lock);
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
  return topicList;*/

}


/**
 * @param Takes a topic to check
 * @return an int value corresponding to the number of messages for the topic provided,
 * returns 0 if topic does not exist
 */
inline int CustomMap::CountFunction(string topic)
{
  /*int messageCount = 0;
  if (structNotEmpty()) {
    if (TopicExists(topic)) {
      shared_lock<shared_mutex> mutex(lock);
      messageCount = dataStructure->at(topic)->size();
    }
  }
  return messageCount;*/

}


/**
 * @param Topic corresponding to the post, the messageID for the int id where the message is saved
 * @return the string value of the message, if no message is found, a blank string will be returned
 */
inline string CustomMap::ReadFunction(string topic, int messagedID)
{
  /*string message="";
  if (structNotEmpty()) {
    if (TopicExists(topic)) {
      shared_lock<shared_mutex> mutex(lock);
      if (dataStructure->at(topic)->size() > messagedID) {
        message = dataStructure->at(topic)->at(messagedID);
      }
    }
  }
  return message;*/
}

/**
 * @param Topic to be searched for
 * @return bool value, True if the topic is found, False if the topic does not exist
 */
inline bool CustomMap::TopicExists(string topic) {
  {
    //shared_lock<shared_mutex> mutex(lock);

    char* chars = new char[topic.length() + 1];

    strcpy(chars, topic.c_str());
    HashNode<char*, CustomVector<char*>>* topicArray;

    int index = hash(chars);
    {
      shared_lock<shared_mutex> mutex(lock);
      topicArray = vec->getItem(index);
    }
    {
      shared_lock<shared_mutex> mutex(topicArray->lock);

      while (topicArray->getKey() != chars || topicArray->getNext() != nullptr) {
        topicArray = topicArray->getNext();
      }
      if (topicArray->getKey() == chars) {
        delete[] chars;
        return true;
      }
      else {
        delete[] chars;
        return false;
      }
    }

  }
}

inline bool CustomMap::structNotEmpty() {
  {
    if (vec->getCapacity() == 0) {
      return false;
    }
    else return true;
  }
}

unsigned int CustomMap::hash(char* str)
{
  unsigned int hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c;

  return hash % vec->getSize();
}