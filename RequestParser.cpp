#include <regex>

#include "RequestParser.h"

PostRequest::PostRequest() : Request(0)
{ }

PostRequest::~PostRequest()
{ }

Request* PostRequest::parse(std::string request)
{
	std::regex postRegex("^POST(@[^@#]*)#(.+)$");
	std::smatch postMatch;
	PostRequest* post = new PostRequest();

	if (!std::regex_match(request, postMatch, postRegex, std::regex_constants::match_default))
	{
		post->valid = 0;
		return post;
	}

	post->topicId = postMatch[1];
	post->message = postMatch[2];
	post->valid = 1;

	return post;
}

std::string PostRequest::getTopicId()
{
	return topicId;
}

std::string PostRequest::getMessage()
{
	return message;
}

std::string PostRequest::toString()
{
	return std::string("POST") + topicId + std::string("#") + message;
}

ReadRequest::ReadRequest() : Request(0)
{ }

ReadRequest::~ReadRequest()
{ }

Request* ReadRequest::parse(std::string request)
{
	std::regex readRegex("^READ(@[^@#]*)#([0-9]+)$");
	std::smatch readMatch;
	ReadRequest* read = new ReadRequest();

	if (!std::regex_match(request, readMatch, readRegex, std::regex_constants::match_default))
	{
		read->valid = 0;
		return read;
	}

	read->topicId = readMatch[1];
	read->postId = std::stoi(readMatch[2]);
	read->valid = 1;

	return read;
}

std::string ReadRequest::getTopicId()
{
	return topicId;
}

int ReadRequest::getPostId()
{
	return postId;
}

std::string ReadRequest::toString()
{
	return std::string("READ") + topicId + std::string("#") + std::to_string(postId);
}

CountRequest::CountRequest() : Request(0)
{ }

CountRequest::~CountRequest()
{ }

Request* CountRequest::parse(std::string request)
{
	std::regex countRegex("^COUNT(@[^@#]*)$");
	std::smatch countMatch;
	CountRequest* count= new CountRequest();

	if (!std::regex_match(request, countMatch, countRegex, std::regex_constants::match_default))
	{
		count->valid = 0;
		return count;
	}

	count->topicId = countMatch[1];
	count->valid = 1;

	return count;
}

std::string CountRequest::getTopicId()
{
	return topicId;
}

std::string CountRequest::toString()
{
	return std::string("COUNT") + topicId;
}

ListRequest::ListRequest() : Request(0)
{ }

ListRequest::~ListRequest()
{ }

Request* ListRequest::parse(std::string request)
{
	std::regex listRegex("^LIST$");
	std::smatch listMatch;
	ListRequest* list = new ListRequest();

	if (!std::regex_match(request, listMatch, listRegex, std::regex_constants::match_default))
	{
		list->valid = 0;
		return list;
	}

	list->valid = 1;

	return list;
}

std::string ListRequest::toString()
{
	return std::string("LIST");
}

ExitRequest::ExitRequest() : Request(0)
{ }

ExitRequest::~ExitRequest()
{ }

Request* ExitRequest::parse(std::string request)
{
	std::regex exitRegex("^EXIT$");
	std::smatch exitMatch;
	ExitRequest* exitReq = new ExitRequest();

	if (!std::regex_match(request, exitMatch, exitRegex, std::regex_constants::match_default))
	{
		exitReq->valid = 0;
		return exitReq;
	}

	exitReq->valid = 1;

	return exitReq;
}

std::string ExitRequest::toString()
{
	return std::string("EXIT");
}
