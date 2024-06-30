#include "Directory.h"
#include <fstream>

class FileHandler
{
public:
	FileHandler();
	~FileHandler();

	Directory GetNextDirectory();

private:
	const char* _filename = "dirlist.txt";
	std::ifstream _dirfile;
};
