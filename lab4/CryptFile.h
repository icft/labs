#pragma once
#ifndef CRYPTFILE_H
#define CRYPTFILE_H

#include "File.h"
#include "FileSystem.h"
#include "kuznechik.h"

/*!
*       \brief Данный класс, описывает основные функции и переменные для работы с зашифрованными файлами
*/
class CryptFile : public File 
{
private:
    std::map<std::string, users> user_table;
    uint8_t file_key[32];
    //    std::map<std::string, int> orig_sz;

    //    void uuencoder(std::iostream &from, std::iostream &to);
    //    void uudecoder(std::iostream &from, std::iostream &to);
public:
    CryptFile(std::map<std::string, users> user_table);
    /*!
    Создание зашифрованного файла
    \param[in] filename название файла
    \param[in] stream_name название файлового потока
    \param[in] content информация
    \param[in] content_size размер
    \param[in] owner имя пользователя*/
    bool create(std::string filename, std::string stream_name, char* content, int content_size, std::string owner);
    /*!
    Изменить зашифрованный файл
    \param[in] stream название файлового потока
    \param[in] contest информация
    \param[in] content_size размер
    \param[in] owner имя пользователя
    */
    void edit(std::string stream, char* content, int content_size, std::string owner);
    /*!
    Добавить файловый поток
    \param[in] название файлового потока
    \param[in] информация
    \param[in] размер
    */
    void add_stream_to_file(std::string stream, char* content, int content_size);
    //    void set_access(std::string user_name, access_rights);
    /*!
    Получить информацию
    \param[in] название файлового потока
    \param[in] имя пользователя
    \param[in] длина
    */
    char* get_data(std::string stream, std::string user, int* out_len);
};

#endif // CRYPTFILE_H


