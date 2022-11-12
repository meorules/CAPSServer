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

	if (!std::regex_match(request, postMatch, postRegex, std::regex_constants::match_default))
	{
		this->valid = 0;
		return this;
	}

	this->topicId = postMatch[1];
	this->message = postMatch[2];
	this->valid = 1;

	return this;
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

	if (!std::regex_match(request, readMatch, readRegex, std::regex_constants::match_default))
	{
		this->valid = 0;
		return this;
	}

	this->topicId = readMatch[1];
	this->postId = std::stoi(readMatch[2]);
	this->valid = 1;

	return this;
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

	if (!std::regex_match(request, countMatch, countRegex, std::regex_constants::match_default))
	{
		this->valid = 0;
		return this;
	}

	this->topicId = countMatch[1];
	this->valid = 1;

	return this;
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

	if (!std::regex_match(request, listMatch, listRegex, std::regex_constants::match_default))
	{
		this->valid = 0;
		return this;
	}

	this->valid = 1;

	return this;
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

	if (!std::regex_match(request, exitMatch, exitRegex, std::regex_constants::match_default))
	{
		this->valid = 0;
		return this;
	}

	this->valid = 1;

	return this;
}

std::string ExitRequest::toString()
{
	return std::string("EXIT");
}
