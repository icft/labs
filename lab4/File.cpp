#include "File.h"
#include <ctime>
#include <exception>
#include <list>
#include <string>
#include <sstream>
void File::create(std::string nm, std::string stream, std::string s, std::string own) {
	name = nm;
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
	rights[own] = { true, true, true };
	size[stream] = s.length();
}

void File::edit(std::string stream, std::string s) {
	Map<std::string, std::string>::Iterator it = data.find(stream);
	if (it == data.end())
		throw std::exception("Stream doesn't exist");
	it.edit_value(s);
	size[stream] = s.length();
}

std::string File::get_data(std::string stream) {
	Map<std::string, std::string>::Iterator it = data.find(stream);
	if (it == data.end())
		throw std::exception("Stream doesn't exist");
	return it.get_value();
}

void File::add_access(std::string s, access_rights ac) {
	rights[s] = ac;
}

void File::edit_access(std::string s, access_rights ac) {
	std::map<std::string, access_rights>::iterator it = rights.find(s);
	if (it == rights.end())
		throw std::exception("User doesn't exist");
	rights[s] = ac;
}

std::string File::get_sizes() const {
	std::string s="";
	std::map<std::string, int>::const_iterator it = size.cbegin();
	for (; it != size.cend(); it++) {
		std::stringstream ss;
		ss << it->second;
		std::string p;
		ss >> p;
		s += "Stream-" + it->first + "; " + "Size-" + p;
		s += "\n";
	}
	return s;
}

std::ostream& operator<<(std::ostream& s, const File& f) {
	s << "Name: " << f.name << "\n";
	s << "Time of creation: " << f.create_time->tm_hour << ":" << f.create_time->tm_min << "\n";
	s << "Date of creation: " << f.create_time->tm_mday << "." << f.create_time->tm_mon << "." << f.create_time->tm_year << "\n";
	s << "Time of last modification: " << f.last_modify->tm_hour << ":" << f.last_modify->tm_min << "\n";
	s << "Date of last modification: " << f.last_modify->tm_mday << "." << f.last_modify->tm_mon << "." << f.last_modify->tm_year << "\n";
	s << f.get_sizes();
	s << "Owner: " << f.owner;
}


