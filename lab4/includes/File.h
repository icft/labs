#pragma once

#include <ctime>
#include <string>
#include <list>
#include "Node.h"
#include "CryptFile.h"

typedef struct access_rights {
	bool can_read;
	bool can_write;
	bool can_execute;
} access;

class File : public Node { 
private:
	FILE* file;
	tm create;
	tm last_modify;
	std::string owner;
	std::list<std::pair<std::string, access>> rights;
	size_t size;
public:
	~File();
	friend std::istream& operator>> (std::istream&, File&);
	void edit();
	friend std::ostream& operator<< (std::ostream&, const File&);
	void operator+= (std::string);
	void edit_access();
	CryptFile& encrypt();
	void rename();
};

