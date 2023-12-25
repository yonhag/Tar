#pragma once
#include <string>
#include <fstream>

class FileHandler
{
public:
	FileHandler();
	~FileHandler();
	std::string GetNextDirectoryIP();

private:
	std::ifstream _directoryListFile;
	const std::string _directoryFileName = "dirfile.txt";
};