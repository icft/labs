#pragma once

#include <ctime>
#include <string>
#include "File.h"
#include "List.h"
#include "Node.h>

class crypt_file : public node {
private:
	std::unique_ptr<FILE> data;
	tm create;
	tm last_modify;
	std::string owner;
	list<std::pair<std::string, access>> rights;
	size_t size;
public:
	~crypt_file();
	friend std::istream& operator>> (std::istream&, crypt_file&);
	void edit();
	friend std::ostream& operator<< (std::ostream&, const crypt_file&);
	void operator+= (std::string);
	void edit_access();
	ffile& decrypt();
	void rename();
};

