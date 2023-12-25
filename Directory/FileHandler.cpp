#include "FileHandler.h"

FileHandler::FileHandler()
{
	this->_directoryListFile.open(this->_directoryFileName, std::ios::in);

	if (!this->_directoryListFile.is_open())
		throw std::exception("File cannot be open");
}

FileHandler::~FileHandler()
{
	this->_directoryListFile.close();
}

std::string FileHandler::GetNextDirectoryIP()
{
	std::string ip;
	std::getline(this->_directoryListFile, ip);
	return ip;
}

