#pragma once
#include "Map.h"
#include <list>
#include "CryptFile.h"

typedef struct access_rights {
	bool can_read;
	bool can_write;
	bool can_execute;
} access;

class File {
public:
	std::string name;
	int offset;
	Map<std::string, std::string> data;
	tm* create_time;
	tm* last_modify;
	std::string owner;
	Map<std::string, access> rights;
	Map<std::string, int> size;


	File() = default;
	~File() = default;
	void create(std::string, std::string, std::string, std::string, int);
	void edit(std::string, std::string);
	std::string get_data(std::string);
	void add_access(std::string, access_rights);
	void edit_access(std::string, access_rights);
	void rename(std::string);
};



