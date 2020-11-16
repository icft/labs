#pragma once

#include <list>

enum Type {
	file,
	dir,
	cryptfile,
	execute,
};

class Node {
private:
	Type type; 
public:
	std::string get_name();
};

