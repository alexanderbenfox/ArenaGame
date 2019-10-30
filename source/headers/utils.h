#ifndef utils_h
#define utils_h

#ifdef _WIN32
#undef min
#undef max
#endif

#include <string>
#include <vector>

class Utils {
public:
	
	//unsigned int split
	//split a string <txt> everywhere a certain character <ch> is found
	//store the resulting substrings in a vector <strs>
	//returns the size of the vector
	// ex. std::string "25,46,3,64" -> split by ch ',' -> store in &strs -> return # of tokens
  static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch);

  static std::string GetWorkingPath();

  static std::string CorrectPathForSystem(const char* path);
	
};



#endif /* utils_h */
