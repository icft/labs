#pragma once

#include "Node.h"
#include <string>
#include "List.h"

typedef struct user {
	std::string username;
	std::string password;
	uint8_t key[32];
} user;

class FileSystem {
private:
	node* root;
	list<user> table;
public:
	FileSystem();
	~FileSystem();
	//for files
	void create_file(std::string);
	void print(std::string);
	void edit(std::string);
	void close(std::string);
	void print_file_info(std::string);
	void type_file(std::string);
	void get_access_rights(std::string);
	void edit_access_rights(std::string);
	void encrypt(std::string);
	void decrypt(std::string);
	uint8_t* generate_key();
	//for directories
	void create_dir(std::string);
	void read_dir(std::string);
	void copy_dir(std::string, std::string);
	void move_dir(std::string, std::string);
	void del_dir(std::string);
	//for files and directories
	void rename(std::string);
	//for fs
	void create_user(std::string, std::string);
	void print_fs();
	void edit_users();
	void print_statistics();
};

