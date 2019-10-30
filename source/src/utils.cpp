#include "utils.h"
#include <direct.h>
#include <algorithm>

unsigned int Utils::split(const std::string &txt, std::vector<std::string> &strs, char ch) {
  int pos = (int)txt.find(ch);
  int initialPos = 0;
  strs.clear();

  //While we have found an instance of ch
  while (pos != std::string::npos) {
    strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
    initialPos = pos + 1;

    pos = (int)txt.find(ch, initialPos);
  }
  // Add the last one
  strs.push_back(txt.substr(initialPos, std::min<int>(pos, (int)txt.size() - (initialPos + 1))));

  return (int)strs.size();
}

std::string Utils::GetWorkingPath()
{
  char temp[256];
  return (_getcwd(temp, sizeof(temp)) ? std::string(temp) : std::string(""));
}


std::string Utils::CorrectPathForSystem(const char* path)
{
#ifdef _WIN32
  std::string pathStr = path;
  size_t loc = pathStr.find('/');
  if (loc != std::string::npos)
  {
    pathStr.replace(pathStr.begin(), pathStr.begin() + loc, "..\\..");
  }
  std::replace(pathStr.begin(), pathStr.end(), '/', '\\');
  return pathStr;
#endif
  return path;
}