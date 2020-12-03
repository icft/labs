#pragma once
#include <list>
#include <string>
#include "File.h"
#include "Map.h"
#include <iostream>

class Dir : public File {
public:
	Map<std::string, File*> elems;

	Dir() = default;
	~Dir() = default;
	void print() {
		Map<std::string, File*>::ConstIterator it = elems.cbegin();
		for (; it != elems.cend(); it++) {
			if (data.empty())
				std::cout << name << "--- dir\n";
			else
				std::cout << name << "--- file\n";
		}
	}
};
