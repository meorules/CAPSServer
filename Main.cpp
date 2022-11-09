#include "TCPServer.h"
#include "TCPClient.h"
#include "ThreadPool.h"
#include "RequestParser.h"
#include "DataStructureAPI.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

/* To do List:
*
- Create separate functions for parsing requests and also processing requests, that way they can be done independently or together if need be
This will allow for the parts to work seamlessly and make it easier to test thread pooling vs just using normal threads more easily. 
- Create a new request parser that works with strings
- Custom data structure, an array containing an pointers for each topic to various arrays. 
This will allow the arrays to be accessed on their own and the critical section to be very small, it also means
once the pointer to the array of messages in a topic is found, it can unlock the "dictionary" array/ data structure 
This will also mean another lock on the array for each specific topic. 
- Test the thread-pooling for all the tasks, for split tasks and normal threading
- Test the normal request parser vs Strings request parser
- Test the std::unordered_map vs my own implementation
*/

#define DEFAULT_PORT 12345
#define preMadeParser true


bool terminateServer = false;


int main()
{
	TCPServer server(DEFAULT_PORT);

	ReceivedSocketData receivedData;
	ThreadPool* threadPool = new ThreadPool();
	//std::vector<std::thread> serverThreads;

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	while (!terminateServer)
	{
		receivedData = server.accept();

		if (!terminateServer)
		{

			//parse the data and then do something with it
			//serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
			//The function first and then the arguements
		}
	}


	/*for (auto& th : serverThreads)
		th.join();*/

	std::cout << "Server terminated." << std::endl;
	delete threadPool;
	return 0;
}

ReceivedSocketData&& parseRequest(TCPServer* server, ReceivedSocketData&& data) {
	unsigned int socketIndex = (unsigned int)data.ClientSocket;

	server->receiveData(data, 0);
	/*PostRequest post = PostRequest::parse(data.request);
	if (post.valid)
	{
		std::cout << "Post request: " << post.toString() << std::endl;
		std::cout << "Post topic: " << post.getTopicId() << std::endl;
		std::cout << "Post message: " << post.getMessage() << std::endl;
	}

	ReadRequest read = ReadRequest::parse(data.request);
	if (read.valid)
	{
		std::cout << "Read request" << read.toString() << std::endl;
		std::cout << "Read topic: " << read.getTopicId() << std::endl;
		std::cout << "Read post id: " << read.getPostId() << std::endl;
		
	}
	CountRequest count = CountRequest::parse(data.request);
	if (count.valid)
	{
		std::cout << "Count request: " << count.toString() << std::endl;
		std::cout << "Count topic: " << count.getTopicId() << std::endl;
	}

	ListRequest list = ListRequest::parse(data.request);
	if (list.valid)
	{
		std::cout << "List request: " << list.toString() << std::endl;
	}

	ExitRequest exitReq = ExitRequest::parse(data.request);
	if (exitReq.valid)
	{
		std::cout << "Exit request: " << exitReq.toString() << std::endl;

	}*/

	
}

