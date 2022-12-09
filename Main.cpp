#include "TCPServer.h"
#include "TCPClient.h"
#include "ReceivedSocketData.h"
#include "RequestParser.h"
#include "StringRequestParser.h"
#include "DataStructureAPI.h"
#include "UnorderedMap.h"
#include "CustomMapNOTWORKING/CustomMap.h"
#include "topicLockMap.h"
#include "NoLockUnorderedMap.h"
#include "UnorderedMapStringArray.h"

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

#define DEFAULT_PORT 12345
//#define preMadeParser



//DataStructureAPI* bigDataStructure = new UnorderedMapStringArray();

bool terminateServer = false;




void parseRequest(TCPServer* server, ReceivedSocketData&& data) {
	unsigned int socketIndex = (unsigned int)data.ClientSocket;

	StringRequestParser parser;
	ParsedRequest* request;
	DataStructureAPI* dataStructure = new NoLockUnorderedMap();

	
	do {
		server->receiveData(data, true);
		
		request = parser.parseRequest(data.request);

		switch (request->requestCommand) {
		case requestToBeParsed::notSet: {
			data.reply = "";
			break;
		}
		case requestToBeParsed::post: {
			int id = dataStructure->PostFunction(request->getTopic(), request->getMessage());
			data.reply = to_string(id);
			server->sendReply(data);
			break;
		}
		case requestToBeParsed::read: {
			string message = dataStructure->ReadFunction(request->getTopic(), request->getMessageID());
			data.reply = message;
			server->sendReply(data);
			break;
		}
		case requestToBeParsed::list: {
			string topicList = dataStructure->ListFunction();
			data.reply = topicList;
			server->sendReply(data);
			break;
		}
		case requestToBeParsed::count: {
			int noOfMessages = dataStructure->CountFunction(request->getTopic());
			data.reply = std::to_string(noOfMessages);
			server->sendReply(data);
			break;
		}
		case requestToBeParsed::ex: {
			data.reply = "Terminating";
			terminateServer = true;
			server->sendReply(data);
			break;
		}
		}

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