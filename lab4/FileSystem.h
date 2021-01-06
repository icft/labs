#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "Map.h"
#include "File.h"
#include "Dir.h"
#include "Struct.h"

class FileSystem {
private:
    Dir* root;
    std::mutex m_root;
    std::map<std::string, users> table; // таблица юзеров
    void print_fs_recurs(Dir* d, int level, bool verbose);
protected:
    Dir* find_dir(std::vector<std::string> dirs, std::string own);
    void del_recurs(Dir* d);
public:
    FileSystem() {
        root = new Dir("/", "admin");
        table["admin"] = { "123", {0} };
        for (int i = 0; i < 32; i++)
            table["admin"].key[i] = i;
    }
    ~FileSystem() {
        this->del("/", "admin");
        delete root;
    }
    // for user management
    void create_user(std::string user, std::string password, std::string own);
    void edit_users_password(std::string name, std::string password);
    void delete_user(std::string name, std::string own);

    //for files
    std::pair<std::vector<std::string>, std::string> parser(std::string);
    bool create_file(std::string name, std::string stream, const char* content, int content_size, std::string own);
    bool create_crypted_file(std::string name, std::string stream, char* content, int content_size, std::string own);
    char* read_file(std::string filename, std::string stream_name, std::string own, int* out_len);
    char* read_crypted_file(std::string filename, std::string stream_name, std::string own, int* out_len);
    bool add_info(std::string name, std::string stream, char* content, int content_size, std::string owner);
    bool edit(std::string name, std::string stream, char* content, int content_size, std::string own);
    bool copy_file(std::string name, std::string path_to, std::string own);

    //for directories
    bool create_dir(std::string name, std::string own);
    std::vector<std::string> read_dir(std::string name, std::string own);
    // copy_dir, dirname and to_dirname - should be FULL PATH, and we can rename the dest dir.
    bool copy_dir(std::string dirname, std::string to_dirname, std::string own);
    //for files and directories
    bool copy(std::string name, std::string path_to, std::string own);
    bool is_dir(std::string filename, std::string own);
    bool del(std::string name, std::string own);
    void print_access_rights(std::string name);
    bool edit_access_rights(std::string name, std::string user, access_rights ac, std::string own);
    bool move(std::string name_file, std::string to_dir, std::string own);
    bool rename(std::string name_file, std::string to_name, std::string own);
    //    void add_rights(std::string name, std::string user, access_rights ac);
    //for fs
    void ls(std::string path); // print current dir
//    bool cd(std::string path, std::string owner); // change current dir
    void print_file_info(std::string name, std::string owner);
    void print_fs(bool verbose);
    void print_statistics();
    void save_fs_state(std::string filename);
    void load_fs_state(std::string filename);
};
