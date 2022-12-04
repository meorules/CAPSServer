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
  CustomVector<HashNode<std::string,CustomVector<std::string>>>* vec;
  unsigned int hash(std::string str);

  //std::string stringToChar(string toConvert);
  bool structNotEmpty();

};

inline CustomMap::CustomMap() {
  vec = new CustomVector<HashNode<std::string, CustomVector<std::string>>>(100000);

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


  HashNode<std::string, CustomVector<std::string>>* topicArray;
  int index = hash(topic);


    if (TopicExists(topic)) {
      {
        topicArray = vec->getItem(index);

      }
      if (topicArray != nullptr) {
        {
          while (topicArray->getKey() != topic || topicArray->getNext() != nullptr) {
            topicArray = topicArray->getNext();
          }
          if (topicArray->getKey() == topic) {
            topicArray->getValue().push(message);
            return topicArray->getValue().getCapacity() - 1;
          }
        }
      }
    }
    else {
      HashNode<std::string, CustomVector<std::string>>* topicArray = new HashNode<std::string, CustomVector<std::string>>(topic, CustomVector<std::string>(100));
        topicArray->getValue().push(message);
      {
        HashNode<std::string, CustomVector<std::string>>* currentArray = vec->getItem(index);
        if (currentArray == nullptr) {
          vec->insert(topicArray, index);
        }
        else {
          while (currentArray->getNext() != nullptr) {
            currentArray = currentArray->getNext();
          }
          currentArray->setNext(topicArray);
        }
        return 0;
      }
    }

}


/**
 * @return a string containing the topics list seperated by #
 */
inline string CustomMap::ListFunction()
{
  HashNode<std::string, CustomVector<std::string>>* topicArray;
  string topicList = ""; 
  int vecSize = 0;
  if (structNotEmpty()) {
    int i = 0;
    int vecSize = vec->getSize();
    while (i < vecSize) {
      {
        topicArray = vec->getItem(i);
      }
      if (topicArray != nullptr) {
        do {
          topicList.append(topicArray->getKey());
          topicList.append("#");
          topicArray = topicArray->getNext();
        } while (topicArray != nullptr);
      }
      i++;
    }
  }
  return topicList;


}


/**
 * @param Takes a topic to check
 * @return an int value corresponding to the number of messages for the topic provided,
 * returns 0 if topic does not exist
 */
inline int CustomMap::CountFunction(string topic)
{
  HashNode<std::string, CustomVector<std::string>>* currentArray;
  bool found = false;

  int messageCount = 0;
  
    if (TopicExists(topic)) {
      int index = hash(topic);
      {
        currentArray = vec->getItem(index);
      }
      if (currentArray != nullptr) {
        do {
          if (currentArray->getKey() == topic) {
            messageCount = currentArray->getValue().getCapacity();
            found = true;
          }
          else {
            if (currentArray->getNext() != nullptr) {
              currentArray = currentArray->getNext();
            }
          }
        } while (!found);
      }
    }
  
  return messageCount;

}


/**
 * @param Topic corresponding to the post, the messageID for the int id where the message is saved
 * @return the string value of the message, if no message is found, a blank string will be returned
 */
inline string CustomMap::ReadFunction(string topic, int messagedID)
{
  
  HashNode<std::string, CustomVector<std::string>>* currentArray;
  string  message = "";
  bool found = false;

  
    if (TopicExists(topic)) {
      int index = hash(topic);
      {
        currentArray = vec->getItem(index);
      }
      if (currentArray != nullptr) {
        do {
          if (currentArray->getKey() == topic) {

            message = *currentArray->getValue().getItem(messagedID);
            found = true;
          }
          else {
            if (currentArray->getNext() != nullptr) {
              currentArray = currentArray->getNext();
            }
          }
        } while (!found);
      }
    }
  

  return message;
}

/**
 * @param Topic to be searched for
 * @return bool value, True if the topic is found, False if the topic does not exist
 */
inline bool CustomMap::TopicExists(string topic) {
  
    if (structNotEmpty()) {
      HashNode<std::string, CustomVector<std::string>>* topicArray;

      int index = hash(topic);
      {
        topicArray = vec->getItem(index);
      }
      {
        if (topicArray != nullptr) {

          while (topicArray->getKey() != topic || topicArray->getNext() != nullptr) {
            topicArray = topicArray->getNext();
          }
          if (topicArray->getKey() == topic) {
            return true;
          }
          else {
            return false;
          }

        }}
    }
    else {
      return false;
    }
  
}

inline bool CustomMap::structNotEmpty() {
  {
    if (vec->getCapacity() == 0) {
      return false;
    }
    else {
     return true;
    }
  }
}

unsigned int CustomMap::hash(std::string str)
{
  unsigned int hash = 5381;
  int c;
  int i = 0;
  while (c = str[i++])
    hash = ((hash << 5) + hash) + c;

  return hash % vec->getSize();
}

//std::string CustomMap::stringToChar(string toConvert) {
//  std::string toConvertChars = new char[toConvert.length() + 1];
//  strcpy(toConvertChars, toConvert.c_str());
//  return toConvertChars;
//}