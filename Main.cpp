#include "TCPServer.h"
#include "TCPClient.h"
//#include "ThreadPool.h"
#include "RequestParser.h"
#include "StringRequestParser.h"
#include "DataStructureAPI.h"
#include "UnorderedMap.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

/* To do List:
*
* 
- Create a new request parser that works with strings
- Custom data structure, an array containing a pointer for each topic to various arrays. 
This will allow the arrays to be accessed on their own and the critical section to be very small, it also means
once the pointer to the array of messages in a topic is found, it can unlock the "dictionary" array/ data structure 
This will also mean another lock on the array for each specific topic.
-Lock free Data Structure - https://github.com/Qarterd/Honeycomb/blob/master/src/common/Honey/Thread/LockFree/UnorderedMap.h
- Test the thread-pooling for all the tasks, for split tasks and normal threading
- Test the normal request parser vs Strings request parser
- Test the std::unordered_map vs my own implementation
*/

#define DEFAULT_PORT 12345
//#define preMadeParser

DataStructureAPI* dataStructure = new UnorderedMap();

bool terminateServer = false;




void parseRequest(TCPServer* server, ReceivedSocketData&& data) {
	unsigned int socketIndex = (unsigned int)data.ClientSocket;
	
	do {
		server->receiveData(data, false);

#ifdef preMadeParser
		bool requestProcessed = false;

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

		requestProcessed = true;

		if (requestProcessed) {
			server->sendReply(data);
	}
		request = NULL;
		requestProcessed = false;

	//server->closeClientSocket(data);
#else
		StringRequestParser parser;
		ParsedRequest* request;
		request = parser.parseRequest(data.request);
		/*enum requestToBeParsed {
			notSet, post, read, list, count, exit
		};*/
		switch (request->requestCommand) {
		case requestToBeParsed::notSet: {
			data.reply = "";
			break;
		}
		case requestToBeParsed::post: {
			int id = dataStructure->PostFunction(request->getTopic(), request->getMessage());
			data.reply = to_string(id);
			break;
		}
		case requestToBeParsed::read: {
			string message = dataStructure->ReadFunction(request->getTopic(), request->getMessageID());
			data.reply = message;
			break;
		}
		case requestToBeParsed::list: {
			string topicList = dataStructure->ListFunction();
			data.reply = topicList;
			break;
		}
		case requestToBeParsed::count: {
			int noOfMessages = dataStructure->CountFunction(request->getTopic());
			data.reply = std::to_string(noOfMessages);
			break;
		}
		case requestToBeParsed::ex: {
			data.reply = "Terminating";
			break;
		}
		}
		server->sendReply(data);
		
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
	TCPServer server(DEFAULT_PORT);


	ReceivedSocketData receivedData;
	//ThreadPool* threadPool = new ThreadPool();
	std::vector<std::thread> serverThreads;

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	while (!terminateServer)
	{
		receivedData = server.accept();

		if (!terminateServer)
		{

			//parse the data and then do something with it
			//serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
			// 
			//The function first and then the arguements
			/*serverThreads.emplace_back(parseRequest, &server, receivedData);*/
			serverThreads.emplace_back(parseRequest, &server, receivedData);
			//threadPool->QueueJob(parseRequest, &server, receivedData);
			//parseRequest(&server, receivedData);
		}
	}


	for (auto& th : serverThreads)
		th.join();

	//threadPool->Stop();

	std::cout << "Server terminated." << std::endl;
	//delete threadPool;
	return 0;
}