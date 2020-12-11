#pragma once

#include <string>
#include <vector>
#include "Map.h"
#include "File.h"
#include "Dir.h"
#include <vector>
#include "Cipher.h"

typedef struct user {
	std::string username;
	std::string password;
	uint8_t key[32];
} users;

class FileSystem {
private:
	std::map<std::string, File*> root;
	std::vector<users> table;
public:
	FileSystem() = default;
	~FileSystem() = default;
	//for files
	Dir* HELP(std::pair<std::vector<std::string>, std::string>, int);
	std::pair<std::vector<std::string>, std::string> parser(std::string);
	void create_file(std::string, std::string, std::string, std::string);
	void print(std::string, std::string, std::string);
	void edit(std::string, std::string, std::string, std::string);
	void print_file_info(std::string, std::string);
	void get_access_rights(std::string, std::string);
	void edit_access_rights(std::string, std::string, access_rights, std::string);
	void encrypt(std::string, std::string);
	void decrypt(std::string, std::string);
	uint8_t* generate_key();
	//for directories
	void create_dir(std::string, std::string);
	void read_dir(std::string, std::string);
	void copy_dir(std::string, std::string, std::string);
	void move_dir(std::string, std::string);
	void del_dir(std::string);
	//for files and directories
	void rename(std::string);
	//for fs
	void add_rights(std::string, std::string, access_rights);
	void create_user(std::string, std::string, std::string);
	void edit_users_password(std::string, std::string);
	void delete_user(std::string, std::string);
	void print_fs();
	void print_help(Dir*, std::string);
	void print_statistics();
	int help_statistics(Dir*);
};



