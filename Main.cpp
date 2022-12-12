#include "TCPServer.h"
#include "TCPClient.h"
#include "ReceivedSocketData.h"
#include "RequestParser.h"
#include "StringRequestParser.h"
#include "DataStructureAPI.h"
#include "UnorderedMap.h"
#include "CustomMapNOTWORKING/CustomMap.h"
#include "topicLockMap.h"


#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

/* To do List:
*
This will also mean another lock on the array for each specific topic.
-Lock free Data Structure - https://github.com/Qarterd/Honeycomb/blob/master/src/common/Honey/Thread/LockFree/UnorderedMap.h
- Test the thread-pooling for all the tasks, for split tasks and normal threading
- Test the normal request parser vs Strings request parser
- Test the std::unordered_map vs my own implementation
*/

//#define THREADPOOL
#define DEFAULT_PORT 12345
//#define preMadeParser
//#define CustomMAP

#ifdef CustomMAP
DataStructureAPI* dataStructure = new topicLockMap();
#else 
DataStructureAPI* dataStructure = new UnorderedMap();
#endif

bool terminateServer = false;




void parseRequest(TCPServer* server, ReceivedSocketData&& data) {
  unsigned int socketIndex = (unsigned int)data.ClientSocket;

  do {
    server->receiveData(data, true);
    bool requestProcessed = false;

#ifdef preMadeParser


    PostRequest* request = new PostRequest();
    request->parse(data.request);

    if (request->valid)
    {
      int id = dataStructure->PostFunction(request->getTopicId(), request->getMessage());
      data.reply = to_string(id);
      requestProcessed = true;
    }


    if (!requestProcessed) {
      ReadRequest* request = new ReadRequest();
      request->parse(data.request);
      if (request->valid)
      {

        string message = dataStructure->ReadFunction(request->getTopicId(), request->getPostId());
        data.reply = message;
        requestProcessed = true;
      }
    }

    if (!requestProcessed) {
      ListRequest* request = new ListRequest();
      request->parse(data.request);
      if (request->valid)
      {
        string topicList = dataStructure->ListFunction();
        data.reply = topicList;
        requestProcessed = true;

      }
    }

    if (!requestProcessed) {
      CountRequest* request = new CountRequest();
      request->parse(data.request);
      if (request->valid)
      {

        int noOfMessages = dataStructure->CountFunction(request->getTopicId());
        data.reply = std::to_string(noOfMessages);
        requestProcessed = true;

      }
    }

    if (!requestProcessed) {
      ExitRequest* request = new ExitRequest();
      request->parse(data.request);
      if (request->valid)
      {
        terminateServer = true;
        data.reply = "Terminating";
        requestProcessed = true;

      }
    }



    if (requestProcessed) {
      server->sendReply(data);
    }
    request = NULL;
    requestProcessed = false;

    //server->closeClientSocket(data);
#else
    string request = data.request;
    if (request != "") {
      if (request[0] == 'P') {
        if (request[1] == 'O' && request[2] == 'S' && request[3] == 'T' && request[4] == '@') {
          string messagePart = request.substr(4);
          int hashFound = messagePart.find('#');
          if (hashFound != -1) {
            int id = dataStructure->PostFunction(messagePart.substr(0, hashFound), messagePart.substr(hashFound + 1));
            data.reply = to_string(id);
            server->sendReply(data);
          }
        }
      }
      else if (request[0] == 'R') {
        if (request[1] == 'E' && request[2] == 'A' && request[3] == 'D' && request[4] == '@') {
          string messagePart = request.substr(4);
          int hashFound = messagePart.find('#');
          if (hashFound != -1) {
            string message = dataStructure->ReadFunction(messagePart.substr(0, hashFound), stoi(messagePart.substr(hashFound + 1)));
            data.reply = message;
            server->sendReply(data);
          }
        }
      }
      else if (request == "EXIT" && request == "exit")
      {
        data.reply = "Terminating";
        terminateServer = true;
        server->sendReply(data);
      }
      else if (request == "LIST") {
        string topicList = dataStructure->ListFunction();
        data.reply = topicList;
        server->sendReply(data);
      }
      else if (request[0] == 'C') {
        if (request[1] == 'O' && request[2] == 'U' && request[3] == 'N' && request[4] == 'T' && request[5] == '@') {
          int noOfMessages = dataStructure->CountFunction(request.substr(5));
          data.reply = std::to_string(noOfMessages);
          server->sendReply(data);
        }
      }
    }
#endif


  } while (data.request != "exit" && data.request != "EXIT" && !terminateServer);
  if (!terminateServer && (data.request == "exit" || data.request == "EXIT"))
  {
    terminateServer = true;

    TCPClient tempClient(std::string("127.0.0.1"), DEFAULT_PORT);
    tempClient.OpenConnection();
    tempClient.CloseConnection();
  }

  server->closeClientSocket(data);

}

int main()
{

#ifdef THREADPOOL

  TCPServer server(DEFAULT_PORT);
  ReceivedSocketData receivedData;
  ThreadPool* threadPool = new ThreadPool();

  std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

  while (!terminateServer)
  {
    receivedData = server.accept();

    if (!terminateServer)
    {
      //threadPool->QueueJob(parseRequest,&server, receivedData );

      threadPool->enqueue<std::function<void()>, TCPServer*, ReceivedSocketData&&>(parseRequest, &server, receivedData);
      //threadPool->QueueJob(parseRequest);
    }
  }

  threadPool->Stop();

  std::cout << "Server terminated." << std::endl;
  delete threadPool;
  return 0;

#else
  TCPServer server(DEFAULT_PORT);
  ReceivedSocketData receivedData;
  std::vector<std::thread> serverThreads;

  std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

  while (!terminateServer)
  {
    receivedData = server.accept();

    if (!terminateServer)
    {
      serverThreads.emplace_back(parseRequest, &server, receivedData);
    }
  }


  for (auto& th : serverThreads)
    th.join();

  std::cout << "Server terminated." << std::endl;
  return 0;

#endif

}