#pragma once
#include <string>
#include <vector>
#include "ParsedRequest.h"
#include <locale>

using namespace std;



class StringRequestParser
{
public:
  StringRequestParser() {}
  ~StringRequestParser() {}

  ParsedRequest* parseRequest(string request);

private:


};

