#include "StringRequestParser.h"

//requestToBeParsed{
//  notSet,post, read, list, count, exit


ParsedRequest* StringRequestParser::parseRequest(string request)
{
  ParsedRequest* toBeReturned = new ParsedRequest();

  if (request != "") {
    if (request == "EXIT" && request== "exit")
    {
      toBeReturned->requestCommand = requestToBeParsed::ex;
      return toBeReturned;
    }
    else {
      if (request == "LIST") {
        toBeReturned->requestCommand = requestToBeParsed::list;
        return toBeReturned;
      }
      else {
        if (request[0] == 'P') {
          if (request.size() >= 7) {
            if (request[1] == 'O' && request[2] == 'S' && request[3] == 'T' && request[4] == '@') {
              toBeReturned->requestCommand = requestToBeParsed::post;
              string messagePart = request.substr(4);
              int hashFound = messagePart.find('#');
              if (hashFound != -1) {
                toBeReturned->topicId = messagePart.substr(0, hashFound);
                toBeReturned->message = messagePart.substr(hashFound + 1);
              }
            }
          }

        }
        else if (request[0] == 'R') {
          if (request.size() >= 7) {
            if (request[1] == 'E' && request[2] == 'A' && request[3] == 'D' && request[4] == '@') {
              toBeReturned->requestCommand = requestToBeParsed::read;
              string messagePart = request.substr(4);
              int hashFound = messagePart.find('#');
              if (hashFound != -1) {
                toBeReturned->topicId = messagePart.substr(0, hashFound);
                toBeReturned->messageID = stoi(messagePart.substr(hashFound + 1));
              }
            }
          }

        }
        else if (request[0] == 'C') {
          if (request.size() >= 6) {
            if (request[1] == 'O' && request[2] == 'U' && request[3] == 'N' && request[4] == 'T' && request[5] == '@') {
              toBeReturned->requestCommand = requestToBeParsed::count;
              toBeReturned->topicId = request.substr(5);
            }
          }

        }
      }
    }
  }
  


  return toBeReturned;
}
