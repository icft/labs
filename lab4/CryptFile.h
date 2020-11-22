#pragma once

#include <ctime>
#include <list>
#include <string>
#include "File.h"

class crypt_file: public ffile {
private:
public:
	crypt_file();
	~crypt_file();
	ffile& decrypt();
};


