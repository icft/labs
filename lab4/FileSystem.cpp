#include "FileSystem.h"
#include "Cryptfile.h"

#include <iostream>
#include <fstream>

std::pair<std::vector<std::string>, std::string> FileSystem::parser(std::string name) {
    char sep = '/';
    if (name.find(sep) == name.npos) {
        return std::make_pair(std::vector<std::string>(), name);
    }
    std::vector<std::string> v;
    for (size_t a = 0, b = 0; a != name.npos; a = b)
        v.push_back(name.substr(a + (a != 0), (b = name.find(sep, a + 1)) - a - (a != 0)));
    std::string fname;
    fname = v[v.size() - 1];
    v.pop_back();
    return std::make_pair(v, fname);
}

Dir* FileSystem::find_dir(std::vector<std::string> dirs, std::string own) {
    Dir* d = root;
    if (dirs.size() == 0)
        return root;
    if (dirs.at(0) == "/")
        dirs.erase(dirs.begin());
    if ((dirs.size() == 1) && (dirs.at(0) == ""))
        return root;
    //    std::cout << "Checking dirs: ";
    for (unsigned i = 0; i < dirs.size(); i++) {
        std::string cur_dir_name = dirs.at(i);
        //        std::cout << cur_dir_name << ", ";
        //        d->m_elems.lock();
        auto d2 = d->elems.find(cur_dir_name);
        if (d2 == d->elems.end()) { // no such dir
//            std::cout << "ERROR: dir '" << cur_dir_name << "' doesn't exists." << std::endl;
            return nullptr;
        }
        else {
            if (!d->user_rights[own].can_execute_or_move) {
                //                std::cout << "ERROR: dir '" << cur_dir_name << "' user '"<< own << "'doesn't have rights on it." << std::endl;
                return nullptr;
            }
            //            std::cout << d2->second << std::endl;
            d = static_cast<Dir*>(d2->second);
        }
        //        d->m_elems.unlock();
    }
    return d;
}


bool FileSystem::create_file(std::string name, std::string stream, const char* content, int content_size, std::string own) {
    //    std::cout << "FileSystem::create_file: BEGIN" << std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    //    std::cout << "Filename: " << filename << std::endl;
    auto d = find_dir(dirs, own);
    if (d == nullptr || d->elems.count(name) != 0)
        return false;
    // TODO: add file
    File* f = new File();
    //    f->filename = filename;
    //    f->add_stream_to_file(stream,content);
    //    f->owner = own;
    //    f->set_access(own, {true,true,true});
    f->create(filename, stream, content, content_size, own);
    if (!d->add_file(f, own)) {
        delete f;
        return false;
    }
    //    std::cout << std::endl;
    //    std::cout << "FileSystem::create_file: END" << std::endl;
    return true;
}

bool FileSystem::create_crypted_file(std::string name, std::string stream, char* content, int content_size, std::string own) {
    //    std::cout << "FileSystem::create_file: BEGIN" << std::endl;
    if (own == "admin") {
        std::runtime_error("FS admin can't have crypted files, he hasn't key");
    }
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    auto d = find_dir(dirs, own);
    if (d == nullptr)
        return false;
    CryptFile* f = new CryptFile(table);
    f->create(filename, stream, content, content_size, own);
    if (!d->add_file(dynamic_cast<File*>(f), own)) {
        delete f;
        return false;
    }
    //    std::cout << std::endl;
    //    std::cout << "FileSystem::create_file: END" << std::endl;
    return true;
}

bool FileSystem::add_info(std::string name, std::string stream, char* content, int content_size, std::string own) {
    //    std::cout << "FileSystem::add_info: BEGIN" << std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, own);
    if (d == nullptr)
        return false;
    File* f = d->elems.find(filename)->second;
    if (f->user_rights[own].can_write)
        f->add_stream_to_file(stream, content, content_size);
    else
        return false;
    return true;
}


bool FileSystem::edit(std::string name, std::string stream, char* content, int content_size, std::string own) {
    //    std::cout << "FileSystem::add_info: BEGIN" << std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, own);
    if (d == nullptr)
        return false;
    File* f = d->elems.find(filename)->second;
    f->edit(stream, content, content_size, own);
    return true;
}


bool FileSystem::copy_file(std::string name_file, std::string to_dir, std::string own) {
    //    std::cout<< "FileSystem::copy_file "<< name_file << " to dir "<< to_dir << " by user " << own <<std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name_file);
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    std::pair<std::vector<std::string>, std::string> _pair2 = parser(to_dir);
    std::vector<std::string>& dirs_to = _pair2.first;
    std::string& newfilename = _pair2.second;
    Dir* d = find_dir(dirs, own);
    Dir* d_to = find_dir(dirs_to, own);
    //    if (d != nullptr && d_to != nullptr){
    //        std::lock_guard<std::recursive_mutex> lk(d->m_elems);
    //        std::lock_guard<std::recursive_mutex> lk2(d_to->m_elems);
    //    }
    if (d != nullptr && d_to != nullptr && d->user_rights[own].can_write) {
        if (d->elems.count(filename)) {
            d->m_elems.lock();
            File* f = d->elems[filename];
            d->m_elems.unlock();
            if (f->user_rights[own].can_read && d_to->user_rights[own].can_write) {
                //create new file
                File* nf = new File();
                nf->filename = newfilename;
                nf->create_time = f->create_time;
                nf->owner = own;
                //copy user_rights
                for (auto ur = f->user_rights.cbegin(); ur != f->user_rights.cend(); ur++) {
                    nf->user_rights[ur->first] = ur->second;
                }
                for (auto s = f->data.cbegin(); s != f->data.cend(); s++) {
                    //copy data and sizes of streams
                    nf->data[s->first] = s->second;
                    nf->sz[s->first] = f->sz[s->first];
                }
                nf->update_last_time();
                d_to->m_elems.lock();
                d_to->elems[newfilename] = nf;
                d_to->m_elems.unlock();
            }
            else
                return false;
            return true;
        }
        else
            return false;
    }
    else
        return false;

}


void FileSystem::print_file_info(std::string name, std::string owner) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = root;
    //    std::cout << "Filename: " << filename << std::endl;
    //    std::cout << "Checking dirs: ";
    for (unsigned i = 0; i < dirs.size(); i++) {
        std::string cur_dir_name = dirs.at(i);
        //        std::cout << cur_dir_name << ", ";
        auto d2 = d->elems.find(cur_dir_name);
        if (d2 == d->elems.end()) { // no such dir
            std::cout << "ERROR: dir '" << cur_dir_name << "' doesn't exists." << std::endl;
            return;
        }
        else {
            if (!d->user_rights[owner].can_execute_or_move) {
                std::cout << "ERROR: dir '" << cur_dir_name << "' user '" << owner << "'doesn't have rights on it." << std::endl;
                return;
            }
            //            std::cout << d2->second << std::endl;
            d = static_cast<Dir*>(d2->second);
        }
    }
    //    std::cout << std::endl;
    File* f = d->elems.find(filename)->second;
    //    std::cout << f;
    std::cout << "File '" << f->filename << "', owner '" << f->owner << "', cr.time: " << mktime(f->create_time) << ", mod.time: " << mktime(f->last_modify) << ", streams: " << std::endl;
    for (auto it = f->sz.cbegin(); it != f->sz.cend(); it++) {
        //        std::cout << "  " << it->first << " -> " << it->second << " bytes" << std::endl;
        std::cout << "  Content of '" << it->first << "' (" << std::dec << it->second << " bytes):\t" << f->data[it->first] << std::endl;
    }
}

void FileSystem::print_access_rights(std::string name) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, "admin");
    if (d != nullptr) {
        std::cout << "FileSystem::print_access_rights('" << name << "')" << std::endl;
        auto& t = (d->elems[filename]->user_rights);
        for (auto it = t.begin(); it != t.end(); it++) {
            access_rights ac = it->second;
            std::cout << "User: " << it->first << " " <<
                (ac.can_read ? "r" : "-") <<
                (ac.can_write ? "w" : "-") <<
                (ac.can_execute_or_move ? "x" : "-") <<
                std::endl;
            //            std::cout << "Can read-" << (ac.can_read ? "true", )<< std::endl;
            //            std::cout << "Can write-" << ac.can_write << std::endl;
            //            std::cout << "Can execute-" << ac.can_execute_or_move << std::endl;
        }
    }
}

bool FileSystem::edit_access_rights(std::string name, std::string user, access_rights ac, std::string own) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, user);
    if (d != nullptr) {
        if (own == "admin" || own == d->elems[filename]->owner) {
            d->elems[filename]->set_access(user, ac);
        }
        else {
            return false;
        }
    }
    return true;
}


void FileSystem::create_user(std::string user, std::string password, std::string own) {
    if (own != "admin")
        throw std::runtime_error("Access denied");
    //проверка, что такого пользователя нет
    if (table.count(user)) {
        throw std::runtime_error("User already exists");
    }
    users s;
    //	s.username = user;
    s.password = password;
    //добавить генерацию ключа
    srand(time(0));
    for (int i = 0; i < 32; i++)
        s.key[i] = rand();
    table[user] = s;
    m_root.lock();
    root->user_rights[user] = { true, true, true };
    m_root.unlock();
    //	table.push_back(s);
    //    std::cout << "FileSystem::create_user : user table size=" << table.size() << ", added '" << user << "'" << std::endl;
}

void FileSystem::edit_users_password(std::string name, std::string password) {
    if (table.count(name) != 1)
        throw std::runtime_error("User doesn't exist, or too many users with this name");
    table[name].password = password;
    //    std::cout << "FileSystem::edit_users_password : user table size=" << table.size() << ", updated '" << name <<
    //                 "', current password="<< table[name].password << std::endl;
}

void FileSystem::delete_user(std::string name, std::string own) {
    if (own != "admin")
        throw std::runtime_error("Access denied");
    table.erase(name);
    m_root.lock();
    root->user_rights.erase(name);
    m_root.unlock();
    //    std::cout << "FileSystem::delete_user : user table size=" << table.size() << ", deleted '" << name << "'" << std::endl;
}

void FileSystem::print_fs_recurs(Dir* d, int level, bool verbose = false) {
    std::string indent;
    for (auto i = 0; i < level; i++) indent += "  ";
    for (auto it = d->elems.begin(); it != d->elems.end(); it++) {
        // print files and dirs names
        auto f = it->second;
        std::cout << indent << f->filename << "\t" << ((f->is_dir()) ? "<dir>" : "<file>") << "\towner(" << f->owner << ")\t" << f->sz.size() << " streams" << std::endl;
        if (verbose)
            std::cout << *f << std::endl;
        //if dir, then print it recursevly
        if (f->is_dir()) {
            Dir* d2 = dynamic_cast<Dir*>(f);
            print_fs_recurs(d2, level + 1);
        }
    }


}

void FileSystem::print_fs(bool verbose = false) {
    //    std::cout << "FileSystem::print_fs" << std::endl;
    print_fs_recurs(root, 0, verbose);
}

void FileSystem::ls(std::string path) {
    std::lock_guard<std::mutex> lk(m_root);
    std::cout << "FileSystem::ls " << path << std::endl;
    auto dirs_p = parser(path);
    auto dirs = dirs_p.first;
    auto dir = dirs_p.second;
    if (dir.size()) dirs.push_back(dir);
    //    m_root.lock();
    Dir* d = root;
    for (unsigned i = 0; i < dirs.size(); i++) {
        std::string cur_dir_name = dirs.at(i);
        auto d2 = d->elems.find(cur_dir_name);
        if (d2 == d->elems.end()) { // no such dir
            std::cout << "ERROR: dir '" << cur_dir_name << "' doesn't exists." << std::endl;
            return;
        }
        else {
            d = static_cast<Dir*>(d2->second);
        }
    }
    std::cout << "Dir '/" << path << "' contains:" << std::endl;
    for (auto it = d->elems.cbegin(); it != d->elems.cend(); it++) {
        std::cout << it->first << "\t" << (it->second->is_dir() ? "<dir>" : "<file>") << "\t" << it->second->sz.size() << " streams" << std::endl;
    }
    std::cout << std::endl;
    //    m_root.unlock();
}


bool FileSystem::create_dir(std::string name, std::string own) {
    //    std::lock_guard<std::mutex> lkr(m_root);
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    //    std::cout<< "FileSystem::create_dir "<< name << " for owner " << own <<std::endl;
    std::vector<std::string>& dirs = _pair.first;
    std::string& dirname = _pair.second;
    Dir* d = find_dir(dirs, own);
    //    if (d != nullptr)
    //        std::lock_guard<std::recursive_mutex> lk(d->m_elems);
    if (d != nullptr && d->elems.count(name) == 0) {
        Dir* new_dir = new Dir(dirname, own);
        d->m_elems.lock();
        d->elems[dirname] = static_cast<File*>(new_dir);
        d->m_elems.unlock();
    }
    else {
        return false;
    }
    return true;
}

bool FileSystem::is_dir(std::string name, std::string own) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    // for every dir in _pair.first check exists and access_rights!
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    //    std::cout << "Filename: " << filename << std::endl;
    auto d = find_dir(dirs, own);
    if (d != nullptr && d->elems.count(name) != 0) {
        auto f = d->elems[filename];
        return f->is_dir();
    }
    return false;
}

std::vector<std::string> FileSystem::read_dir(std::string name, std::string own) {
    std::vector<std::string> out;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    //    std::cout<< "FileSystem::read_dir "<< name << " for owner " << own <<std::endl;
    std::vector<std::string>& dirs = _pair.first;
    std::string& dirname = _pair.second;
    dirs.push_back(dirname);
    Dir* d = find_dir(dirs, own);
    if (d != nullptr) {
        for (auto it = d->elems.begin(); it != d->elems.end(); it++)
            out.push_back(it->first);
    }
    return out;

}

char* FileSystem::read_file(std::string name, std::string stream_name, std::string own, int* out_len) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    //    std::cout<< "FileSystem::read_dir "<< name << " for owner " << own <<std::endl;
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, own);
    if (d != nullptr && d->elems.count(filename)) {
        auto f = d->elems[filename];
        return f->get_data(stream_name, own, out_len);
    }
    return nullptr;
}

char* FileSystem::read_crypted_file(std::string name, std::string stream_name, std::string own, int* out_len) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    //    std::cout<< "FileSystem::read_dir "<< name << " for owner " << own <<std::endl;
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, own);
    if (d != nullptr && d->elems.count(filename)) {
        auto f = dynamic_cast<CryptFile*>(d->elems[filename]);
        return f->get_data(stream_name, own, out_len);
    }
    return nullptr;
}

bool FileSystem::copy_dir(std::string dirname, std::string to_dirname, std::string own) {
    //    std::cout<< "FileSystem::copy_file "<< dirname << " to dir "<< to_dirname << " by user " << own <<std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(dirname);
    std::vector<std::string>& dirs = _pair.first;
    std::string& dirname_from = _pair.second;
    std::pair<std::vector<std::string>, std::string> _pair2 = parser(to_dirname);
    std::vector<std::string>& dirs_to = _pair2.first;
    std::string& dirname_to = _pair2.second;
    if (dirname == to_dirname)
        return false;
    Dir* d = find_dir(dirs, own);
    Dir* d_to = find_dir(dirs_to, own);
    //    std::string str_dir_from, str_dir_to;
    //    for (auto it=dirs.begin(); it != dirs.end(); it++)
    //         str_dir_from += it->data() + "/";
    //    str_dir_from += dirname_from;
    //    for (auto it=dirs_to.begin(); it != dirs_to.end(); it++)
    //         str_dir_to += it->data() + "/";
    //    str_dir_to += dirname_to;
    //    if (d != nullptr && d_to != nullptr){
    //        std::lock_guard<std::recursive_mutex> lk(d->m_elems);
    //        std::lock_guard<std::recursive_mutex> lk2(d_to->m_elems);
    //    }
    if (d != nullptr && d_to != nullptr && d->user_rights[own].can_write) {
        if (d->elems.count(dirname_from)) {
            // get dir_from as file
            File* f = d->elems[dirname_from];
            //check user rights
            if (f->is_dir() && f->user_rights[own].can_read && d_to->user_rights[own].can_write) {
                Dir* df = dynamic_cast<Dir*>(f);
                if (df == nullptr)
                    return false;
                // create new dir
                Dir* dn = new Dir(dirname_to, own);
                d->m_elems.lock();
                d_to->elems[dirname_to] = dn;
                d->m_elems.unlock();
                // copy all files and dirs
                for (auto cf = df->elems.begin(); cf != df->elems.end(); cf++) {
                    //                    // if file is dir then copy dir
                    if (cf->second->is_dir()) {
                        //                        if (!copy_dir(str_dir_from+"/"+cf->first, str_dir_to+"/"+cf->first, own))
                        if (!copy_dir(dirname + "/" + cf->first, to_dirname + "/" + cf->first, own))
                            return false;
                    }
                    else { // else copy file
//                            if (!copy_file(str_dir_from+"/"+cf->first, str_dir_to+"/"+cf->first, own))
                        if (!copy_file(dirname + "/" + cf->first, to_dirname + "/" + cf->first, own))
                            return false;
                    }

                }
            }
            else
                return false;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool FileSystem::copy(std::string name, std::string path_to, std::string own) {
    auto pair = parser(name);
    auto fn = pair.second;
    if (name == path_to) {
        return false;
    }
    //    m_root.lock();
    if (is_dir(name, own)) {
        return this->copy_dir(name, path_to + "/" + fn, own);
    }
    else {
        return this->copy_file(name, path_to + "/" + fn, own);
    }
    //    m_root.unlock();
}


bool FileSystem::move(std::string name_file, std::string to_dir, std::string own) {
    //    std::cout<< "FileSystem::move "<< name_file << " to dir "<< to_dir << " by user " << own <<std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name_file);
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, own);
    std::pair<std::vector<std::string>, std::string> _pair2 = parser(to_dir);
    std::vector<std::string>& dirs_to = _pair2.first;
    std::string& dirname = _pair2.second;
    dirs_to.push_back(dirname);
    Dir* d_to = find_dir(dirs_to, own);
    //    if (d != nullptr && d_to != nullptr){
    //        std::lock_guard<std::recursive_mutex> lk(d->m_elems);
    //        std::lock_guard<std::recursive_mutex> lk2(d_to->m_elems);
    //    }
    if (d != nullptr && d_to != nullptr && d->user_rights[own].can_write) {
        if (d->elems.count(filename)) {
            File* f = d->elems[filename];
            if (f->user_rights[own].can_execute_or_move) {
                d->m_elems.lock();
                d->elems.erase(filename);
                d_to->elems[filename] = f;
                d->m_elems.unlock();
            }
            else
                return false;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void FileSystem::del_recurs(Dir* d) {
    //    std::lock_guard<std::recursive_mutex> lk(d->m_elems);
    if (d != nullptr) {
        for (auto it = d->elems.begin(); it != d->elems.end(); it++) {
            auto filename = it->first;
            //            File *f = d->elems[filename];
            File* f = it->second;
            if (f->is_dir()) {
                Dir* d2 = dynamic_cast<Dir*>(f);
                del_recurs(d2);
            }
            for (auto i2 = f->data.begin(); i2 != f->data.end(); i2++) {
                delete i2->second;
            }
            delete f;
        }
        d->m_elems.lock();
        //        d->elems.erase(filename);
        d->elems.clear();
        d->m_elems.unlock();
    }
}

bool FileSystem::del(std::string name, std::string own) {
    std::vector<std::string> out;
    //    std::cout<< "FileSystem::del_dir "<< name << " for owner " << own <<std::endl;
    std::pair<std::vector<std::string>, std::string> _pair = parser(name);
    //    std::cout << 1;
    std::vector<std::string>& dirs = _pair.first;
    //    std::cout << 2;
    std::string& filename = _pair.second;
    //    std::cout << 3;
    Dir* d = find_dir(dirs, own);
    //    std::cout << 4;
    //    if (d != nullptr)
    //        std::lock_guard<std::recursive_mutex> lk(d->m_elems);
    if (d != nullptr && d->elems.count(filename) && d->elems[filename]->user_rights[own].can_execute_or_move) {
        //        std::cout << 5;
        File* f = d->elems[filename];
        //        std::cout << 6;
        if (f->is_dir()) {
            Dir* d2 = dynamic_cast<Dir*>(f);
            del_recurs(d2);
        }
        d->m_elems.lock();
        d->elems.erase(filename);
        d->m_elems.unlock();
        delete f;
        //        std::cout << 7;
    }
    else
        return false;
    return true;
}


bool FileSystem::rename(std::string name_file, std::string to_name, std::string own) {
    std::pair<std::vector<std::string>, std::string> _pair = parser(name_file);
    //    std::cout<< "FileSystem::rename '"<< name_file << "' to '"<< to_name << "' for owner " << own <<std::endl;
    std::vector<std::string>& dirs = _pair.first;
    std::string& filename = _pair.second;
    Dir* d = find_dir(dirs, own);
    if (d != nullptr && d->elems.count(filename) && d->elems[filename]->user_rights[own].can_execute_or_move) {
        File* f = d->elems[filename];
        d->m_elems.lock();
        d->elems.erase(filename);
        f->filename = to_name;
        d->elems[to_name] = f;
        d->m_elems.unlock();
    }
    else {
        return false;
    }
    return true;
}

#include <stdio.h>

void FileSystem::save_fs_state(std::string filename) {
    //    std::cout<< "FileSystem::save_fs_state "<< filename <<std::endl;
    FILE* f = fopen(filename.c_str(), "wb");
    // save user_table
    uint16_t tab_size = table.size();
    //    write(f, &tab_size, sizeof(tab_size));
    fwrite(&tab_size, sizeof(tab_size), 1, f);
    for (auto it = table.begin(); it != table.end(); it++) {
        uint16_t size;
        const char* username = it->first.c_str();
        size = it->first.size();
        fwrite(&size, sizeof(size), 1, f);
        fwrite(username, size, 1, f);
        const char* password = it->second.password.c_str();
        size = it->second.password.size();
        fwrite(&size, sizeof(size), 1, f);
        fwrite(password, size, 1, f);
        fwrite(it->second.key, sizeof(it->second.key), 1, f);
    }
    //save root_dir
    m_root.lock();
    root->save_to_file(f);
    m_root.unlock();    
    fclose(f);
}


void FileSystem::load_fs_state(std::string filename) {
    m_root.lock();
    table.clear();
    delete root;
    //    std::cout<< "FileSystem::load_fs_state "<< filename <<std::endl;
    //    int f = open(filename.c_str(), O_RDONLY);
    FILE* f = fopen(filename.c_str(), "rb");
    // read user_table;
    uint16_t tab_size;
    fread(&tab_size, sizeof(tab_size), 1, f);
    for (int i = 0; i < tab_size; i++) {
        uint16_t size;
        fread(&size, sizeof(size), 1, f);
        char* username = new char[size + 1];
        fread(username, size, 1, f);
        username[size] = 0;
        fread(&size, sizeof(size), 1, f);
        char* password = new char[size + 1];
        fread(password, size, 1, f);
        password[size] = 0;
        users u;
        u.password = password;
        fread(u.key, sizeof(u.key), 1, f);
        table[username] = u;
        std::cout << username << ", pass:" << password << std::endl;
        delete[] username;
        delete[] password;
    }
    // read root dir
    //    root = new Dir("/","admin");
    bool f_type;
    fread(&f_type, sizeof(f_type), 1, f);
    if (f_type) { // if is dir
        root = Dir::read_dir_from_file(f);
    }
    else {
        std::runtime_error("This should not be! No files instead of root dir!");
        //        File *d = File::read_dir_from_file(f);
    }
    fclose(f);
    m_root.unlock();
}
