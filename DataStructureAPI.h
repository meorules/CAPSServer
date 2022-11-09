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

  virtual int PostFunction(string topic, string message) = 0;
  virtual string ListFunction() = 0;
  virtual int CountFunction(string topic) = 0;
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

inline DataStructureAPI* DataStructureAPI::initialiseStructure(bool custom=false) {
  if (custom) {
    return NULL;//Some new custom dataStructure, will be implemented
  }
  else {
    return new UnorderedMap();
  }
}


/**
 * @param the string to be truncated, will truncate strings if they exceed 140 characters
 * @return the truncated string
 */
string DataStructureAPI::truncateString(string toTruncate) {
  if (toTruncate.length() > 140)
    toTruncate.resize(140);
  return toTruncate;
}