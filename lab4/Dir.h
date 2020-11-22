#pragma once

#include <list>
#include <string>
#include "Node.h"
#include "File.h"
#include "Map.h"
class dir : public ffile {
private:
	std::list<std::pair<std::string, access>> rights;
	size_t size;
	Map<std::string, ffile*> elems;
public:
	dir();
	~dir();
	friend std::ostream& operator>> (std::ostream&, const dir&);
};
