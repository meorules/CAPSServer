#pragma once
#include <string>
#include <vector>
#include "ParsedRequest.h"

using namespace std;



class StringRequestParser
{
public:
  StringRequestParser() {}
  ~StringRequestParser() {}

  ParsedRequest* parseRequest(string request);

private:


};

