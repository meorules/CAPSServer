#ifndef __TCPSERVER_H
#define __TCPSERVER_H
//#define BLOCKING

#include "ReceivedSocketData.h"

class TCPServer
{
public:
  TCPServer(unsigned short int port);
  ~TCPServer();
  ReceivedSocketData accept();
  void receiveData(ReceivedSocketData& ret);
  int sendReply(ReceivedSocketData reply);

  void OpenListenSocket();
  void CloseListenSocket();
  int closeClientSocket(ReceivedSocketData& reply);

private:
  SOCKET ListenSocket;
  unsigned short int port;
  std::string portString;

  int* iResult;
  struct pollfd* pollDescriptor;
  int* retval;
};

#endif __TCPSERVER_H
