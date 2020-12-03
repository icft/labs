#pragma once
#include "File.h"
#include <iostream>

class CryptFile : public File {
public:
	CryptFile() = default;
	~CryptFile() = default;
	std::string decrypt(std::string);
	std::string encrypt(std::string);
};



