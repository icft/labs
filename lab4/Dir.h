#pragma once
#include <list>
#include <string>
#include "File.h"
#include "Map.h"
#include <iostream>

/*!
*       \brief Данный класс, описывает основные функции и переменные для работы с директориями
*/
class Dir : public File {
public:
    std::map<std::string, File*> elems;

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
    /*!
    Добавить файл
    \param[in] f указатель на файл
    \param[in] owner имя пользователя
    */
    bool add_file(File* f, std::string owner) {
        auto it = user_rights.find(owner);
        if (it == user_rights.end()) {
            return false;
        }
        auto ar = it->second;
        if (ar.can_execute_or_move && ar.can_write) {
            elems[f->filename] = f;
            return true;
        }
        else {
            return false;
        }

    }
    /*!
    Вывести директорию*/
    void print() {
        //		std::map<std::string, File*>::iterator it = elems.begin();
        for (auto it = elems.begin(); it != elems.end(); it++) {
            if (it->second->is_dir())
                std::cout << it->first << " --- dir\n";
            else
                std::cout << it->first << " --- file\n";
        }
    }
    //    void create(std::string filename){
    //        this->filename = filename;
    //    }
};
