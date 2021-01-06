#pragma once

#include "File.h"
#include "kuznechik.h"
#include "Struct.h"

class CryptFile : public File
{
private:
    std::map<std::string, struct users> user_table;
    uint8_t file_key[32];
    //    std::map<std::string, int> orig_sz;

    //    void uuencoder(std::iostream &from, std::iostream &to);
    //    void uudecoder(std::iostream &from, std::iostream &to);
public:
    CryptFile(std::map<std::string, struct users> user_table);
    //    CryptFile(File*);
    bool create(std::string filename, std::string stream_name, char* content, int content_size, std::string owner);
    void edit(std::string stream, char* content, int content_size, std::string owner);
    void add_stream_to_file(std::string stream, char* content, int content_size);
    //    void set_access(std::string user_name, access_rights);
    char* get_data(std::string stream, std::string user, int* out_len);
};
