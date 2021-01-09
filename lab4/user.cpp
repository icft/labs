#include "user.h"
#include <random>
#include <unistd.h>
#include <chrono>
#include <algorithm>

std::string User::gen_name(int count){
    std::string out;
    for (int i=0; i<count; i++){
        out += 'a' + (char)(random()%26);
    }
    return out;
}

void User::run(int max_count){
    int cnt=0;
//    srand(12);
    while(true){
        cnt++;
//        if (cnt % (max_count/2) == 0) {
//            fs->print_fs(false);
//        }
        if (cnt > max_count)
            break;
        std::this_thread::sleep_for(std::chrono::duration<long, std::milli>(300));
    // generate comand
        int cmd = rand() % 6;
    // generate filename
        auto fn = gen_name(8);
        auto cont = gen_name(100);
        switch ((FS_CMD)(cmd)) {
        case FS_CreateFile:{
            if (user_dirs.size() && random() % 2){
                auto idx = random() % user_dirs.size();
                auto fn2 = user_dirs.at(idx);
                fn = fn2 + "/" + fn;
            }
//            std::cout << " trylock =" << m_std_cout.try_lock() << std::endl;
            m_std_cout.lock();
            std::cout << username << " created file '" << fn << "' ";
//            m_std_cout.unlock();
            auto res = fs->create_file(fn, "MAIN", cont.c_str(), cont.size(), username);
            user_files.push_back(fn);
//            m_std_cout.lock();
            std::cout << ((res)?"OK":"FAIL") << std::endl;
            m_std_cout.unlock();
        }
            break;
        case FS_DeleteFile:
            if (user_files.size()){
                auto idx = random() % user_files.size();
                auto fn2 = user_files.at(idx);
                user_files.erase(user_files.begin()+idx);
                m_std_cout.lock();
                std::cout << username << " deleted file '" << fn << "' (total "<< user_files.size() <<" files)";
//                m_std_cout.unlock();
                auto res = fs->del(fn2, username);
//                m_std_cout.lock();
                std::cout << ((res)?"OK":"FAIL") << std::endl;
                m_std_cout.unlock();
            }
            break;
        case FS_Mkdir:{
            if (user_dirs.size() && random() % 2){
                auto idx = random() % user_dirs.size();
                auto fn2 = user_dirs.at(idx);
                fn = fn2 + "/" + fn;
            }
            m_std_cout.lock();
            std::cout << username << " created dir '/" << fn << "' ";
//            m_std_cout.unlock();
            auto res = fs->create_dir(fn, username);
            user_dirs.push_back(fn);
//            m_std_cout.lock();
            std::cout << ((res)?"OK":"FAIL") << std::endl;
            m_std_cout.unlock();
        }
            break;
        case FS_Readdir_n_delete:{
#ifdef DEBUG
            std::cout << "FS_Readdir_n_delete\n";
#endif
            auto root_ls = fs->read_dir("/", username);
            if (root_ls.size()){
                auto idx = random() % root_ls.size();
                fn = root_ls.at(idx);
//                std::cout << "FS_Readdir_n_delete, root_ls["<<idx<<"]="<<fn<< " is_dir=" << std::boolalpha << fs->is_dir(fn, username) <<"\n";
                if (fs->is_dir(fn, username) && (random() % 2)){
//                    std::cout << "FS_Readdir_n_delete, try fs->read_dir(fn, username)\n";
                    auto dir_ls = fs->read_dir(fn, username);
//                    std::cout << "FS_Readdir_n_delete, dir_ls.size="<< dir_ls.size() <<"\n";
                    if (dir_ls.size()){
                        idx = random() % dir_ls.size();
                        auto fn2 = dir_ls.at(idx);
//                        std::cout << "FS_Readdir_n_delete, dir_ls["<<idx<<"]="<<fn2<<"\n";
                        fn = fn + "/" + fn2;
                    }
                }
            }
            m_std_cout.lock();
            std::cout << username << " del of '/" << fn << "' ";
//            m_std_cout.unlock();
            auto res = fs->del(fn,username);
            auto p = std::find(user_dirs.cbegin(), user_dirs.cend(), fn);
            if (p != user_dirs.cend())
                user_dirs.erase(p);
            p = std::find(user_files.cbegin(), user_files.cend(), fn);
            if (p != user_files.cend())
                user_files.erase(p);
//            m_std_cout.lock();
            std::cout << ((res)?"OK":"FAIL") << std::endl;
            m_std_cout.unlock();
        }
            break;
        case FS_Readdir_n_copy_file:{
#ifdef DEBUG
            std::cout << "FS_Readdir_n_copy_file\n";
#endif
            auto root_ls = fs->read_dir("/", username);
//            std::cout << "root_ls.size()=" << root_ls.size() << std::endl;
            if (root_ls.size()==1){
                fn = root_ls.at(0);
            }else if (root_ls.size()){
                auto idx = random() % root_ls.size();
                fn = root_ls.at(idx);
                if (fs->is_dir(fn, username) && (random() % 2)){
                    auto dir_ls = fs->read_dir(fn, username);
                    if (dir_ls.size()){
                        idx = random() % dir_ls.size();
                        auto fn2 = dir_ls.at(idx);
                        fn = fn + "/" + fn2;
                    }
                }
            }else
                break;
            std::string dir_name = "";
//            std::cout << "fn= "<< fn << " user_dirs.size()=" << user_dirs.size() << std::endl;
            if (user_dirs.size()){
                auto idx = random() % user_dirs.size();
                dir_name = user_dirs.at(idx);
            }else
                break;
//            std::cout << "fn= "<< fn << " dirname=" << dir_name /*<<" trylock =" << m_std_cout.try_lock()*/ << std::endl;
            m_std_cout.lock();
            std::cout << username << " FS_Readdir_n_copy_file of '/" << fn << "' to '/" << dir_name << "' ";
//            m_std_cout.unlock();
            auto res = fs->copy(fn, dir_name, username);
//            m_std_cout.lock();
            std::cout << ((res)?"OK":"FAIL") << std::endl;
            m_std_cout.unlock();
        }
            break;
        case FS_Readdir_n_move_file:{
            auto root_ls = fs->read_dir("/", username);
            if (root_ls.size()){
                auto idx = random() % root_ls.size();
                auto fn = root_ls.at(idx);
                if (fs->is_dir(fn, username) && (random() % 2)){
                    auto dir_ls = fs->read_dir(fn, username);
                    if (dir_ls.size()){
                        idx = random() % dir_ls.size();
                        auto fn2 = dir_ls.at(idx);
                        fn = fn + "/" + fn2;
                    }
                }
            }
            std::string dir_name = "";
            if (user_dirs.size()){
                auto idx = random() % user_dirs.size();
                dir_name = user_dirs.at(idx);
            }
            m_std_cout.lock();
            std::cout << username << " FS_Readdir_n_move_file of '/" << fn << "' to '/" << dir_name << "' ";
//            m_std_cout.unlock();
            auto res = fs->move(fn, dir_name, username);
//            m_std_cout.lock();
            std::cout << ((res)?"OK":"FAIL") << std::endl;
            m_std_cout.unlock();
        }
            break;
        default:
            m_std_cout.lock();
            std::cout << username << " in Panic!!!!\n";
            m_std_cout.unlock();
        }
    }
}
