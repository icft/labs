#pragma once
#include "Map.h"
#include <list>
#include <map>

typedef struct access_rights {
	bool can_read;
	bool can_write;
	bool can_execute_or_move;
} access;

class File {
public:
	std::string name;
	Map<std::string, std::string> data;
	tm* create_time;
	tm* last_modify;
	std::string owner;
	std::map<std::string, access> rights;
	std::map<std::string, int> size;


	File() = default;
	virtual ~File() = default;
	void create(std::string, std::string, std::string, std::string);
	void create_dir(std::string nm, std::string own) {
		name = nm;
		owner = own;
		time_t now = time(0);
		create_time = localtime(&now);
		last_modify = localtime(&now);
		rights[own] = { true, true, true };
	}
	void edit_last_time() {
		time_t now = time(0);
		last_modify = localtime(&now);
	}
	void edit(std::string, std::string);
	void add_access(std::string, access_rights);
	void edit_access(std::string, access_rights);
	void rename(std::string);
	bool can_read(std::string s) { return rights[s].can_read == 1; }
	bool can_write(std::string s) { return rights[s].can_write == 1; }
	bool can_ex_or_move(std::string s) { return rights[s].can_execute_or_move == 1; }
	std::string get_data(std::string);
	friend std::ostream& operator<<(std::ostream&, const File&);
	std::string get_sizes() const;
	bool is_dir() {
		if (data.empty())
			return true;
		else 
			return false;
	}
};

class CryptFile : public File {
public:
	std::map<std::string, std::string> cipher_data;
};


