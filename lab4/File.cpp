#include "File.h"
#include <ctime>
#include <exception>
#include <list>

void File::create(std::string name, std::string stream, std::string s, std::string own, int of) {
	offset = of;
	time_t now = time(0);
	create_time = localtime(&now);
	last_modify = localtime(&now);
	owner = own;
	Map<std::string, std::string>::Iterator it = data.find(stream);
	if (it != data.end())
		throw std::exception("Stream exist");
	std::pair<std::string, std::string> d;
	d.first = stream;
	d.second = s;
	data.add(d);
	std::pair<std::string, access> _pair;
	_pair.first = own;
	_pair.second = { true, true, true };
	rights.add(_pair);
	std::pair<std::string, int> p;
	p.first = stream;
	p.second = s.length();
	size.add(p);
}

void File::edit(std::string stream, std::string s) {
	Map<std::string, std::string>::Iterator it = data.find(stream);
	if (it == data.end())
		throw std::exception("Stream doesn't exist");
	it.edit_value(s);
	Map<std::string, int>::Iterator it1 = size.find(stream);
	it1.edit_value(s.length());
}

std::string File::get_data(std::string stream) {
	Map<std::string, std::string>::Iterator it = data.find(stream);
	if (it == data.end())
		throw std::exception("Stream doesn't exist");
	return it.get_value();
}

void File::add_access(std::string s, access_rights ac) {
	std::pair<std::string, access_rights> _pair;
	_pair.first = s;
	_pair.second = ac;
	rights.add(_pair);
}

void File::edit_access(std::string s, access_rights ac) {
	Map<std::string, access_rights>::Iterator it = rights.find(s);
	if (it == rights.end())
		throw std::exception("User doesn't exist");
	it.edit_value(ac);
}

void File::rename(std::string s) {
	name = s;
}
