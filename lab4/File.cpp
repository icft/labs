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

void File::save_to_file(FILE* f) {
    // save isdir
    fwrite(&this_is_dir, sizeof(this_is_dir), 1, f);
    // save filename
    uint16_t size = filename.size();
    fwrite(&size, sizeof(size), 1, f);
    fwrite(filename.c_str(), size, 1, f);
    // save create_time
    time_t t = mktime(create_time);
    fwrite(&t, sizeof(t), 1, f);
    // save last_modify
    t = mktime(last_modify);
    fwrite(&t, sizeof(t), 1, f);
    // save owner
    size = owner.size();
    fwrite(&size, sizeof(size), 1, f);
    fwrite(owner.c_str(), size, 1, f);
    // save user_rights
    // save count users
    size = user_rights.size();
    fwrite(&size, sizeof(size), 1, f);
    // save each user rights
    for (auto it = user_rights.begin(); it != user_rights.end(); it++) {
        // save username
        size = it->first.size();
        fwrite(&size, sizeof(size), 1, f);
        fwrite(it->first.c_str(), size, 1, f);
        //save access
        fwrite(&(it->second), sizeof(it->second), 1, f);
    }
    // save streams
    size = data.size();
    fwrite(&size, sizeof(size), 1, f);
    // save each stream
    for (auto it = data.begin(); it != data.end(); it++) {
        // save stream name
        auto stream_name = it->first;
        size = stream_name.size();
        fwrite(&size, sizeof(size), 1, f);
        fwrite(stream_name.c_str(), size, 1, f);
        //save data len
        fwrite(&(sz[stream_name]), sizeof(sz[stream_name]), 1, f);
        //save data
        fwrite(it->second, sz[stream_name], 1, f);
    }
}

File* File::read_dir_from_file(FILE* f) {
    // assuming, that type already have been readed
// load filename
    uint16_t size;
    fread(&size, sizeof(size), 1, f);
    char* filename = new char[size + 1];
    fread(filename, size, 1, f);
    filename[size] = 0;
    // load create_time
    time_t t_cr, t_lm;
    fread(&t_cr, sizeof(t_cr), 1, f);
    // load last_modify
    fread(&t_lm, sizeof(t_lm), 1, f);
    // load owner
    fread(&size, sizeof(size), 1, f);
    char* owner = new char[size + 1];
    fread(owner, size, 1, f);
    owner[size] = 0;
    File* file = new File();
    file->filename = filename;
    file->owner = owner;
    delete[] filename;
    delete[] owner;
    file->create_time = localtime(&t_cr);
    file->last_modify = localtime(&t_lm);
    // load user_rights
    // load count users
    uint16_t user_count;
    fread(&user_count, sizeof(user_count), 1, f);
    for (int i = 0; i < user_count; i++) {
        fread(&size, sizeof(size), 1, f);
        char* username = new char[size + 1];
        fread(username, size, 1, f);
        username[size] = 0;
        access ac;
        fread(&ac, sizeof(ac), 1, f);
        file->user_rights[username] = ac;
        delete[] username;
    }
    // load streams -- zero streams for dir
    uint16_t records_count;
    fread(&records_count, sizeof(records_count), 1, f);
    for (int i = 0; i < records_count; i++) {
        //load stream name
        fread(&size, sizeof(size), 1, f);
        char* stream_name = new char[size + 1];
        fread(stream_name, size, 1, f);
        stream_name[size] = 0;
        //load stream size
        fread(&size, sizeof(size), 1, f);
        //load data
        char* data = new char[size];
        fread(data, size, 1, f);
        file->data[stream_name] = data;
        file->sz[stream_name] = size;
        delete[]  stream_name;
    }
    return file;
}
