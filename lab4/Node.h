#pragma once

#include <list>

enum type {
	file,
	dir,
	cryptfile,
	execute,
};

class node {
private:
	type type; 
public:
	std::string get_name();
};

