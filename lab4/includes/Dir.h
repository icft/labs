#pragma once

#include <list>
#include <string>
#include "Node.h"
#include "File.h"

class Dir : public Node {
private:
	std::list<std::pair<std::string, access>> rights;
	size_t size;
	std::list<Node*> elems;
public:
	~Dir();
	friend std::ostream& operator>> (std::ostream&, const Dir&);
	void rename();
};
