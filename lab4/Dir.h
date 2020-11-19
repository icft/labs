#pragma once

#include <list>
#include <string>
#include "Node.h"
#include "File.h"

class dir : public node {
private:
	std::list<std::pair<std::string, access>> rights;
	size_t size;
	std::list<node*> elems;
public:
	~dir();
	friend std::ostream& operator>> (std::ostream&, const dir&);
	void rename();
};
