#pragma once
#ifndef DIR_H
#define DIR_H

#include <list>
#include <string>
#include "Cryptfile.h"
#include "File.h"
#include "Map.h"
#include <iostream>
#include <time.h>
#include <mutex>
//#include "Cryptfile.h"

class Dir : public File {
public:
    std::map<std::string, File*> elems;
    std::recursive_mutex m_elems;
	
    Dir(std::string dirname, std::string own) {
        filename = dirname;
        this_is_dir = true;
        owner = own;
        time_t now = time(0);
        create_time = localtime(&now);
        last_modify = localtime(&now);
        user_rights[own] = { true, true, true };
        if (own != "admin")
            user_rights["admin"] = { true, true, true };
    }
	~Dir() = default;
    bool add_file(File *f, std::string owner){
//        std::lock_guard<std::recursive_mutex> lk(m_elems);
        auto it = user_rights.find(owner);
        if (it == user_rights.end()){
            return false;
        }
        auto ar = it->second;
        if (ar.can_execute_or_move && ar.can_write){
            m_elems.lock();
            elems[f->filename] = f;
            m_elems.unlock();
            return true;
        } else {
            return false;
        }

    }
	void print() {
//		std::map<std::string, File*>::iterator it = elems.begin();
        std::lock_guard<std::recursive_mutex> lk(m_elems);
        for (auto it = elems.begin(); it != elems.end(); it++) {
            if (it->second->is_dir())
                std::cout << it->first << " --- dir\n";
			else
                std::cout << it->first << " --- file\n";
		}
	}
    void save_to_file(FILE *f){
        //save count of records
        std::lock_guard<std::recursive_mutex> lk(m_elems);
        dynamic_cast<File*>(this)->save_to_file(f);
        uint16_t dir_size = elems.size();
        fwrite(&dir_size, sizeof(dir_size), 1 ,f);
        //for each record...
        for(auto it=elems.begin(); it!=elems.end(); it++){
            //check is_dir
            if (it->second->is_dir()){
            //save dir
                Dir *d = dynamic_cast<Dir*>(it->second);
                d->save_to_file(f);
            }else{
                //save file
                it->second->save_to_file(f);
            }

        }
    }
    static Dir *read_dir_from_file(FILE *f){
        // assuming, that type already have been readed
   // load filename
        uint16_t size;
        fread(&size, sizeof(size), 1, f);
        char *filename = new char[size+1];
        fread(filename, size, 1, f);
        filename[size] = 0;
    // load create_time
        time_t t_cr, t_lm;
        fread(&t_cr, sizeof(t_cr), 1, f);
    // load last_modify
        fread(&t_lm, sizeof(t_lm), 1, f);
    // load owner
        fread(&size, sizeof(size), 1, f);
        char *owner = new char[size+1];
        fread(owner, size, 1, f);
        owner[size] = 0;
        Dir *d=new Dir(filename, owner);
        d->create_time = localtime(&t_cr);
        d->last_modify = localtime(&t_lm);
        // load user_rights
        // load count users
        uint16_t user_count;
        fread(&user_count, sizeof(user_count), 1, f);
        for (int i=0; i< user_count; i++){
            fread(&size, sizeof (size),1,f);
            char* username = new char[size+1];
            fread(username, size, 1,f);
            username[size] = 0;
            access_rights ac;
            fread(&ac, sizeof(ac), 1, f);
            d->user_rights[username] = ac;
            delete[] username;
        }
    // load streams -- zero streams for dir
        fread(&size, sizeof(size),1,f);
        if (size != 0){
            std::runtime_error("Dir::read_dir_from_file: dir cannot hold steams");
        }
   // load dir records
        uint16_t records_count;
        fread(&records_count, sizeof(records_count),1,f);
        for (int i=0; i < records_count; i++){
            //load file type
            bool f_type;
            fread(&f_type, sizeof(f_type), 1, f);
            File *fp;
            if (f_type){ // if is dir
                Dir *d2 = Dir::read_dir_from_file(f);
                fp = dynamic_cast<File *>(d2);
            }else{
                fp = File::read_dir_from_file(f);
//                if (fp->sz.count("SYMKEY")){
//                    // this is crypt file
////                    CryptFile *cf = new CryptFile(fp);
////            CryptFile *cf;
//                    cf = dynamic_cast<CryptFile*>(fp);
//                    if (fp){
//                        std::cout << "dyn _cast ok = " << cf;
//                    }else{
//                        std::cout << "dyn _cast fail = " << cf;
//                    }
                //                    fp = reinterpret_cast<File *>(cf);
                //                    fp = static_cast<File *>(cf);
//                }
            }
            d->elems[fp->filename] = fp;
        }
        delete [] filename;
        delete [] owner;
        return d;
    }
};

#endif
