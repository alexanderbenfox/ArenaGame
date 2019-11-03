#ifndef utils_h
#define utils_h

#ifdef _WIN32
#undef min
#undef max
#endif

#include <string>
#include <vector>

template<typename T>
class Point2D
{
public:
  Point2D() = default;
  Point2D(T x, T y) : _x(x), _y(y) {}

  Point2D operator+(const Point2D<T>& pt)
  {
    return Point2D<T>(pt._x + _x, pt._y + _y);
  }

  T X() { return _x; }
  T Y() { return _y; }

private:
  T _x;
  T _y;

  friend class Point2D<T>;

};

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

  static Point2D<int> Conver2DToIso(int x, int y);
	
};



#endif /* utils_h */
