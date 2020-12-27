#include "File.h"
#include <ctime>
//#include <exception>
//#include <stdexcept>
//#include <list>
//#include <string>
//#include <sstream>


bool File::create(std::string nm, std::string stream, char* content, int content_size, std::string own) {
    filename = nm;
    time_t now = time(0);
    this->create_time = localtime(&now);
    this->last_modify = localtime(&now);
    this->owner = own;
    //    Map<std::string, std::string>::Iterator it = data.find(stream);
    if (stream != "MAIN" && data.is_empty())
        return false;
    auto it = data.find(stream);
    if (it != data.end())
        throw std::runtime_error("Stream exist");
    //	std::pair<std::string, std::string> d;
    //	d.first = stream;
    //    d.second = content;
    //    data.add(d);
    data[stream] = new char[content_size];
    sz[stream] = content_size;
    memcpy(data[stream], content, content_size);
    user_rights["admin"] = { true, true, true };
    user_rights[own] = { true, true, true };
    return true;
}

void File::edit(std::string stream, char* content, int content_size, std::string user) {
    //    Map<std::string, std::string>::Iterator it = data.find(stream);
    auto it = data.find(stream);
    if (it == data.end())
        throw std::runtime_error("Stream doesn't exist");
    //    it.edit_value(content);
    if (user_rights.count(user) == 1) {
        it->second = new char[content_size];
        memcpy(it->second, content, content_size);
    }
    sz[stream] = content_size;
}

char* File::get_data(std::string stream, std::string user, int* out_len) {
    //    Map<std::string, std::string>::Iterator it = data.find(stream);
    auto it = data.find(stream);
    if (it == data.end())
        throw std::runtime_error("Stream doesn't exist");
    if (user_rights.count(user) == 1) {
        *out_len = sz[stream];
        return it->second;
        //    return it.get_value();
    }
    else {
        *out_len = -1;
        return nullptr;
    }
}


void File::set_access(std::string user_name, access_rights ac) {
    user_rights[user_name] = ac;
}


std::ostream& operator<<(std::ostream& s, const File& f) {
    s << "Time of creation: " << f.create_time->tm_hour << ":" << f.create_time->tm_min << "\n";
    s << "Date of creation: " << f.create_time->tm_mday << "." << f.create_time->tm_mon << "." << f.create_time->tm_year << "\n";
    s << "Time of last modification: " << f.last_modify->tm_hour << ":" << f.last_modify->tm_min << "\n";
    s << "Date of last modification: " << f.last_modify->tm_mday << "." << f.last_modify->tm_mon << "." << f.last_modify->tm_year << "\n";
    for (auto it = f.sz.begin(); it != f.sz.end(); it++) {
        s << "Stream '" << it->first << "' size " << it->second;
    }
    //    s << f.get_sizes_for_print();
    s << "Owner: " << f.owner;
    return s;
}



