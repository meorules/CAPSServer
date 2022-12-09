#pragma once
#include <string>
using namespace std;
enum requestToBeParsed {
  notSet,post, read, list, count, ex
};

class ParsedRequest
{
public:
  ParsedRequest(): requestCommand(notSet),topicId(""),message(""),messageID(-1) {}
  ~ParsedRequest() {};

  string getTopic();
  string getMessage();
  int getMessageID();

  friend class StringRequestParser;

  requestToBeParsed requestCommand;

  

private:
  string topicId;
  string message;
  int messageID;
};

 inline string ParsedRequest::getTopic() {
   if (requestCommand == notSet) {
     return "";
   }
   else {
     return topicId;
   }
}

 inline string ParsedRequest::getMessage() {
   if (requestCommand == notSet) {
     return "";
   }
   else {
     return message;
   }
 }

 inline int ParsedRequest::getMessageID() {
   if (requestCommand == notSet) {
     return -1;
   }
   else {
     return messageID;
   }
 }
