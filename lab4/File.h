#pragma once

#include "Map.h"
#include <list>
#include <map>
#include <string.h>

typedef struct {
    bool can_read;
    bool can_write;
    bool can_execute_or_move;
} access_rights;

class File {
public:
    std::string filename;
    //    Map<std::string, std::string> data; // stream_name:data
    Map<std::string, char*> data; // stream_name:data
//    std::map<std::string, std::string> data; // stream_name:data
    std::map<std::string, uint16_t> sz; // stream_name:stream_size
    tm* create_time;
    tm* last_modify;
    std::string owner;
    std::map<std::string, access_rights> user_rights;
    bool this_is_dir;


    File() = default;
    virtual ~File() = default;
    //	void copy(); //не реализовано
    bool is_dir() {
        return this->this_is_dir;
    }

    bool create(std::string filename, std::string stream_name, const char* content, int content_size, std::string owner);

    void update_last_time() {
        time_t now = time(0);
        last_modify = localtime(&now);
    }
    void edit(std::string stream, char* content, int content_size, std::string owner);
    void add_stream_to_file(std::string stream, char* content, int content_size) {
        data[stream] = new char[content_size];
        sz[stream] = content_size;
        memcpy(data[stream], content, content_size);
    }
    void set_access(std::string user_name, access_rights);
    bool can_read(std::string username) { return user_rights[username].can_read == 1; }
    bool can_write(std::string username) { return user_rights[username].can_write == 1; }
    bool can_exec_or_move(std::string username) { return user_rights[username].can_execute_or_move == 1; }
    char* get_data(std::string stream, std::string user, int* out_len);
    // for debug print
    friend std::ostream& operator<<(std::ostream&, const File&);
    //    std::string get_sizes_for_print() const;
    void save_to_file(FILE* f);
    static File* read_dir_from_file(FILE* f);
};
