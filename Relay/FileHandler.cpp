#include "FileHandler.h"
#include <iostream> // #TODO: Remove this 

FileHandler::FileHandler()
{
	this->_dirfile.open(this->_filename, std::ios::in);

	if (!this->_dirfile.is_open())
		throw std::runtime_error("File cannot be open");
}

FileHandler::~FileHandler()
{
	this->_dirfile.close();
}

/*
* Returns directories one by one from dirfile.txt
* Returns empty Directory object if reached EOF
*/
Directory FileHandler::GetNextDirectory()
{
	Directory dir;

	std::string ip;
	if (std::getline(this->_dirfile, ip, ' ')) // Separating by space to only get the ip part
		dir.ip = ip;
	else
		return dir;

	std::cout << "IP: " << ip << std::endl;

	std::string port;
	if (std::getline(this->_dirfile, port)) // Separating by newline character to get the port part
		dir.port = stoul(port);

	std::cout << "Port: " << dir.port << std::endl;

	return dir;
}
