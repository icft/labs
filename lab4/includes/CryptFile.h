#pragma once

#include <ctime>
#include <list>
#include <string>
#include "File.h"

class CryptFile
{
private:
	FILE* file;
	tm create;
	tm last_modify;
	std::string owner;
	std::list<std::pair<std::string, access>> rights;
	size_t size;
public:
	~CryptFile();
	friend std::istream& operator>> (std::istream&, CryptFile&);
	void edit();
	friend std::ostream& operator<< (std::ostream&, const CryptFile&);
	void operator+= (std::string);
	void edit_access();
	File& decrypt();
	void rename();
};

