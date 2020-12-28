#pragma once

#include <string>
#include <vector>
#include "Map.h"
#include "File.h"
#include "Dir.h"
#include <vector>
//#include "Cipher.h"

/*!
*       \brief Структура, описывающая пользователя
*/
typedef struct users {
    //	std::string username;
    std::string password;
    uint8_t key[32];
} users;

#include "Cryptfile.h"

/*!
    \brief Класс, описывающий файловую систему и функции работы с ней
*/
class FileSystem {
private:
    Dir* root;
    std::map<std::string, users> table; // таблица юзеров
    void print_fs_recurs(Dir* d, int level, bool verbose);
protected:
    //	Dir* HELP(std::pair<std::vector<std::string>, std::string>, int, std::string);
    //	void del_help(Dir*, std::string);
    //	void print_help(Dir*, std::string);
    //	std::pair<int, int> statistics_help(Dir*);
    /*!
    Функция поиска директории
    */
    Dir* find_dir(std::vector<std::string> dirs, std::string own);
    /*!
    Вспомогательная функция для удаления
    */
    void del_recurs(Dir* d);
public:
    FileSystem() {
        root = new Dir("/", "admin");
        table["admin"] = { "123", {0} };
        for (int i = 0; i < 32; i++)
            table["admin"].key[i] = i;
    }
    ~FileSystem() = default;
    // for user management
    /*!
    Проверяет доступ к ФС
    \param[in] login имя пользователя
    \param[in] password пароль
    */
    void check_user(std::string login, std::string password);
    /*!
    Создание пользователя
    \param[in] user имя пользователя
    \param[in] password пароль
    \param[in] own имя пользователя, который создает
    */
    void create_user(std::string user, std::string password, std::string own);
    /*!
    Смена пароля
    \param[in] name имя пользователя
    \param[in] password пароль
    */
    void edit_users_password(std::string name, std::string password);
    /*!
    Удалить пользователя
    \param[in] name имя пользователя
    \param[in] own имя пользователя, который удаляет
    */
    void delete_user(std::string name, std::string own);

    //for files
    /*!
    Разбивает путь на компоненты
    */
    std::pair<std::vector<std::string>, std::string> parser(std::string);
    /*!
    Создание файла
    \param[in] name название файла
    \param[in] stream название файлового потока
    \param[in] content информация
    \param[in] content_size размер
    \param[in] own имя пользователя
    */
    bool create_file(std::string name, std::string stream, char* content, int content_size, std::string own);
    /*!
    Создание зашифрованного файла
    \param[in] name название файла
    \param[in] stream название файлового потока
    \param[in] content информация
    \param[in] content_size размер
    \param[in] own имя пользователя
    */
    bool create_crypted_file(std::string name, std::string stream, char* content, int content_size, std::string own);
    /*!
    Прочитать файл
    \param[in] filename название файла
    \param[in] stream_name название файлового потока
    \param[in] own имя пользователя
    \param[in] out_len размер
    */
    char* read_file(std::string filename, std::string stream_name, std::string own, int* out_len);
    /*!
    Прочитать зашифрованный файл
    \param[in] filename название файла
    \param[in] stream_name название файлового потока
    \param[in] own имя пользователя
    \param[in] out_len размер
    */
    char* read_crypted_file(std::string filename, std::string stream_name, std::string own, int* out_len);
    /*!
    Добавить информацию в файле(по новому потоку)
    \param[in] name название файла
    \param[in] stream название файлового потока
    \param[in] content информация
    \param[in] content_size размер
    \param[in] owner имя пользователя
    */
    bool add_info(std::string name, std::string stream, char* content, int content_size, std::string owner);
    /*!
    Изменить файл
    \param[in] name название файла
    \param[in] stream название файлового потока
    \param[in] content информация
    \param[in] content_size размер
    \param[in] own имя пользователя
    */
    bool edit(std::string name, std::string stream, char* content, int content_size, std::string own);
    /*!
    Копирование файла
    \param[in] name название файла
    \param[in] path_to путь, куда копировать
    \param[in] own имя пользователя
    */
    bool copy_file(std::string name, std::string path_to, std::string own);

    //for directories
    /*!
    Создание директории
    \param[in] name название директории
    \param[in] own имя пользователя
    */
    bool create_dir(std::string name, std::string own);
    /*!
    Удаление директории
    \param[in] name название директории
    \param[in] own имя пользователя
    */
    bool del(std::string name, std::string own);
    /*!
    Прочитать директории
    \param[in] name название директории
    \param[in] own имя пользователя
    */
    std::vector<std::string> read_dir(std::string name, std::string own);
    // copy_dir, dirname and to_dirname - should be FULL PATH, and we can rename the dest dir.
    /*!
    Копирование директории
    \param[in] dirname название директории
    \param[in] to_dirname путь, куда копировать
    \param[in] own имя пользователя
    */
    bool copy_dir(std::string dirname, std::string to_dirname, std::string own);
    //for files and directories
    /*!
    Вывести права доступа по файлу или директории
    \param[in] name название файла или директории
    */
    void print_access_rights(std::string name);
    /*!
    Изменить права доступа по файлу или директории
    \param[in] name название файла или директории
    \param[in] user имя пользователя
    \param[in] ac права доступа
    \param[in] own имя пользователя
    */
    bool edit_access_rights(std::string name, std::string user, access_rights ac, std::string own);
    /*!
    Передвинуть файл или директорию
    \param[in] name_file название файла или директории
    \param[in] to_dir путь, куда передвинуть
    \param[in] own имя пользователя
    */
    bool move(std::string name_file, std::string to_dir, std::string own);
    /*!
    Переименовать файл или директорию
    \param[in] name_file название файла или директории
    \param[in] to_name новое название
    \param[in] own имя пользователя
    */
    bool rename(std::string name_file, std::string to_name, std::string own);
    //    void add_rights(std::string name, std::string user, access_rights ac);
    //for fs
    /*!
    Вывести текущую директорию
    \param[in] path название директории
    */
    void ls(std::string path); // print current dir
//    bool cd(std::string path, std::string owner); // change current dir
    /*!
    Вывести информацию по файлу
    \param[in] name название файла
    \param[in] owner имя пользователя
    */
    void print_file_info(std::string name, std::string owner);
    /*!
    Вывести файловую систему
    \param[in] verbose расширенный вывод */
    void print_fs(bool verbose);
    /*!
    Сохранения ФС в файл
    \param[in] filename название файла
    */
    void save_fs_state(std::string filename);
    /*!
    Чтение ФС из файл
    \param[in] filename название файла
    */
    void load_fs_state(std::string filename);
    /*!*/
    //	void encrypt(std::string, std::string);
    //	void decrypt(std::string, std::string);
    //	uint8_t* generate_key();
};

