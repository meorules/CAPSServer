#ifndef __REQUESTPARSER_H
#define __REQUESTPARSER_H

#include <string>

class Request {
public:
	Request(bool valid): valid(valid){}
	~Request();
	virtual std::string toString() = 0;
	virtual Request* parse(std::string request)=0;


	bool valid;


};

class PostRequest : Request
{
public:
	PostRequest();
	~PostRequest();
	Request* parse(std::string request);
	std::string getTopicId();
	std::string getMessage();
	std::string toString();

	std::string topicId;
	std::string message;
};

class ReadRequest : Request
{
public:
	ReadRequest();
	~ReadRequest();
	Request* parse(std::string request);
	std::string getTopicId();
	int getPostId();
	std::string toString();

	std::string topicId;
	int postId;
};

class CountRequest : Request
{
public:
	CountRequest();
	~CountRequest();
	Request* parse(std::string request);
	std::string getTopicId();
	std::string toString();


	std::string topicId;
};

class ListRequest : Request
{
public:
	ListRequest();
	~ListRequest();
	Request* parse(std::string request);
	std::string toString();

};

class ExitRequest : Request
{
public:
	ExitRequest();
	~ExitRequest();
	Request* parse(std::string request);
	std::string toString();

};

#endif //__REQUESTPARSER_H

