#pragma once

#include <ctime>
#include <string>
#include <list>
#include <memory>
#include "Node.h"
#include "CryptFile.h"
#include "List.h"
#include "FileSystem.h"

typedef struct access_rights {
	bool can_read;
	bool can_write;
	bool can_execute;
} access;

class ffile : public node { 
private:
	std::unique_ptr<FILE> file;
	tm create;
	tm last_modify;
	std::string owner;
	list<std::pair<std::string, access>> rights;
	size_t size;
public:
	~ffile();
	friend std::istream& operator>> (std::istream&, ffile&);
	void edit();
	friend std::ostream& operator<< (std::ostream&, const ffile&);
	void operator+= (std::string);
	void edit_access(int, user);
	crypt_file& encrypt();
	void rename(std::string);
};

