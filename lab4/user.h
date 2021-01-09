#ifndef USER_H
#define USER_H

#include <string>
#include "FileSystem.h"
//#include <thread>

enum FS_CMD {
    FS_CreateFile,
    FS_DeleteFile,
    FS_Mkdir,
//    FS_Readdir_n_create_file,
//    FS_Readdir_n_create_subdir,
    FS_Readdir_n_delete,
    FS_Readdir_n_copy_file,
    FS_Readdir_n_move_file,
};

static std::mutex m_std_cout;

class User
{
private:
    std::string username;
    FileSystem *fs;
    std::vector<std::string> user_dirs;
    std::vector<std::string> user_files;
    static std::string gen_name(int count);
public:
    User(std::string username, FileSystem *fs) {
        this->username = username;
        this->fs = fs;
    }
    void run(int max_count);

};

#endif // USER_H
