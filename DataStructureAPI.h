#pragma once
#include <string>
#include <shared_mutex>

using namespace std;
//This class facilitates the interaction between the request parser and the data structure.
// By keeping it as a separate API, the data structure can be changed from here easily without affecting the request parser in any way. 
class DataStructureAPI
{
public: 
  DataStructureAPI();
  ~DataStructureAPI();

  DataStructureAPI* initialiseStructure(bool custom=false);

  /**
 * @param Topic is the topic for which the post needs to take place, topics are created if they dont exist.
 * message is the message added the topic specified.
 * @return an int value corresponding to the id of the message which was saved
 */
  virtual int PostFunction(string topic, string message) = 0;

  /**
 * @return a string containing the topics list seperated by @ and #
 */
  virtual string ListFunction() = 0;

  /**
 * @param Takes a topic to check
 * @return an int value corresponding to the number of messages for the topic provided,
 * returns 0 if topic does not exist
 */
  virtual int CountFunction(string topic) = 0;

  /**
 * @param Topic corresponding to the post, the messageID for the int id where the message is saved
 * @return the string value of the message, if no message is found, a blank string will be returned
 */
  virtual string ReadFunction(string topic, int messagedID) = 0;


  virtual bool TopicExists(string topic) = 0;
  virtual bool structNotEmpty()=0;

  string truncateString(string toTruncate);


private:

protected:
  shared_mutex lock;

};

inline DataStructureAPI::DataStructureAPI()
{
}

inline DataStructureAPI::~DataStructureAPI()
{
}

//inline DataStructureAPI* DataStructureAPI::initialiseStructure(bool custom=false) {
//  if (custom) {
//    return NULL;//Some new custom dataStructure, will be implemented
//  }
//  else {
//    return new UnorderedMap();
//  }
//}


/**
 * @param the string to be truncated, will truncate strings if they exceed 140 characters
 * @return the truncated string
 */
string DataStructureAPI::truncateString(string toTruncate) {
  if (toTruncate.length() > 140)
    toTruncate.resize(140);
  return toTruncate;
}