#include "TCPServer.h"
#include "TCPClient.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#define DEFAULT_PORT 12345
#define preMadeParser true


bool terminateServer = false;


int main()
{
	TCPServer server(DEFAULT_PORT);

	ReceivedSocketData receivedData;

	//std::vector<std::thread> serverThreads;
	//Possibly create a thread pool, etc to test it out

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	while (!terminateServer)
	{
		receivedData = server.accept();

		if (!terminateServer)
		{
			//parse the data and then do something with it
			//serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
		}
	}


	/*for (auto& th : serverThreads)
		th.join();*/

	std::cout << "Server terminated." << std::endl;

	return 0;
}