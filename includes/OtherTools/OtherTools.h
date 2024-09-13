#pragma once
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sys/stat.h>
#include <vector>

class OtherTools
{
public:
  OtherTools();
  void PrintIntro();
  void PrintInstructions();
  void PrintFileName(const char* argv);
  void PrintOutro();
  int CreateExeBuffer(std::string filepath);
	~OtherTools();

public:
  struct RETVAL { char* buffer = nullptr; unsigned long long size = 0; } retval;

};

