#pragma once
#include "Map.h"
#include <list>
#include <map>
#include <string.h>

/*!
*       \brief Структура, описывающая права пользователей
*/
typedef struct access_rights {
    bool can_read;
    bool can_write;
    bool can_execute_or_move;
} access;

/*!
*       \brief Данный класс, описывает основные функции и переменные для работы с файлами
*/
class File {
public:
    std::string filename;
    //    Map<std::string, std::string> data; // stream_name:data
    Map<std::string, char*> data; // stream_name:data
//    std::map<std::string, std::string> data; // stream_name:data
    std::map<std::string, int> sz; // stream_name:stream_size
    tm* create_time;
    tm* last_modify;
    std::string owner;
    std::map<std::string, access> user_rights;
    bool this_is_dir;


    File() = default;
    virtual ~File() = default;
    /*!Возвращает true, если файл является директорией. В противном случае false*/
    bool is_dir() {
        return this->this_is_dir;
    }
    /*!Создание файла
    \param[in] filename название файла
    \param[in] stream_name название файлового потока
    \param[in] content информация
    \param[in] owner имя пользователя
    */
    bool create(std::string filename, std::string stream_name, char* content, int content_size, std::string owner);
    /*!Обновляет время модификации*/
    void update_last_time() {
        time_t now = time(0);
        last_modify = localtime(&now);
    }
    /*!Изменяет информацию
    \param[in] stream название файлового потока
    \param[in] content информация
    \param[in] owner имя пользователя*/
    void edit(std::string stream, char* content, int content_size, std::string owner);
    /*!Добавляет новый файловый поток
    \param[in] stream название файлового потока
    \param[in] content информация*/
    void add_stream_to_file(std::string stream, char* content, int content_size) {
        data[stream] = new char[content_size];
        sz[stream] = content_size;
        memcpy(data[stream], content, content_size);
    }
    /*!Устанавливает права для пользователя
    \param[in] user_name имя пользователя
    \param[in] access_rights права*/
    void set_access(std::string user_name, access_rights);
    /*!Проверяет права на чтение*/
    bool can_read(std::string username) { return user_rights[username].can_read == 1; }
    /*!Проверяет права на запись*/
    bool can_write(std::string username) { return user_rights[username].can_write == 1; }
    /*!Проверяет права на запуск для файлов или возможность прохода для директорий*/
    bool can_exec_or_move(std::string username) { return user_rights[username].can_execute_or_move == 1; }
    /*!Получить информацию
    \param[in] stream название файлового потока
    \param[in] user имя пользователя*/
    char* get_data(std::string stream, std::string user, int* out_len);
    // for debug print
    /*!Вывод основной информации*/
    friend std::ostream& operator<<(std::ostream&, const File&);
    //    std::string get_sizes_for_print() const;
};
