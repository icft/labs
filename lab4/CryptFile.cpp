#include "Cryptfile.h"
#include <random>
#include <time.h>

CryptFile::CryptFile(std::map<std::string, users> user_table)
{
    this->user_table = user_table;
    srand(time(0));
    for (int i = 0; i < 32; i++)
        file_key[i] = rand();
}

//CryptFile::CryptFile(File* file){
//    this->filename = file->filename;
//    this->this_is_dir = false;
//    this->create_time = file->create_time;
//    last_modify = file->last_modify;
//    owner = file->owner;
//    user_rights = file->user_rights;
//    sz = file->sz;
//    data = file->data;
//    srandom(time(0));
//    for (int i=0; i<32; i++)
//        file_key[i] = rand();
//    std::cout << "CryptFile::CryptFile(File* file), data.size=" << data.size() <<std::endl;
//}


bool CryptFile::create(std::string filename, std::string stream_name, char* content, int content_size, std::string owner) {
    this->filename = filename;
    this_is_dir = false;
    time_t now = time(0);
    this->create_time = localtime(&now);
    this->last_modify = localtime(&now);
    this->owner = owner;
    // check stream name
    if (stream_name == "SYMKEY")
        std::runtime_error("can't work ");
    auto it = data.find(stream_name);
    if (it != data.end())
        throw std::runtime_error("Stream exist");
    // crypt key with owner key
    std::cout << "Key: ";
    for (int i = 0; i < 32; i++) {
        //        ckey[i] = file_key[i];
        std::cout << std::hex << int(file_key[i]);
    }
    std::cout << std::endl;
    //    user_rights["admin"] = { true, true, true };
    if (owner != "admin")
        user_rights[owner] = { true, true, true };

    // init crypt
    kuz_init();
    kuz_key_t sk;
    // crypt data
    kuz_set_encrypt_key(&sk, file_key);
    //    std::cout << "Content: " << content << " (";
    //    for (int i=0; i<content_size; i++){
    //        std::cout << std::hex << int(content[i]);
    //    }
    //    std::cout << ")" << std::endl;
    //    uint32_t out_content_size = content_size;
    //    char* out = kuz_encrypt_ctr(&sk, content, &out_content_size);
    char* out = kuz_encrypt_ctr(&sk, content, content_size);
    //    std::cout << "Content_cry: ";
    //    for (uint32_t i=0; i < out_content_size; i++){
    //        std::cout << std::hex << int(out[i]&0xFF);
    //    }
    //    std::cout << std::endl;
    //    char *out2 = kuz_decrypt(&sk, out, out_content_size);
    //    std::cout << "Content: " << out2 << " (";
    //    for (int i=0; i<content_size; i++){
    //        std::cout << std::hex << int(out2[i]&0xFF);
    //    }
    //    std::cout << ")" << std::endl;

        // store it
    data[stream_name] = out;
    sz[stream_name] = content_size;
    //    sz[stream_name] = out_content_size;
    //    orig_sz[stream_name] = content_size;

    // put crypted keys in SYMKEY
    for (auto it = user_rights.begin(); it != user_rights.end(); it++) {
        if (it->first == "admin")
            continue;
        kuz_set_encrypt_key(&sk, user_table[it->first].key);
        char* out_key = new char[32];
        memcpy(out_key, file_key, 32);
        kuz_encrypt_block(&sk, out_key);
        kuz_encrypt_block(&sk, out_key + 16);
        //        std::cout << "crypted key: " << std::hex;
        //        for (int i=0; i<32; i++){
        //            std::cout << std::hex << (unsigned int)(out_key[i] & 0xFF);
        //        }
        //        std::cout << std::endl;
                //append data
        int offset = sz["SYMKEY"];
        char* new_data = new char[offset + 32];
        memcpy(new_data, data["SYMKEY"], offset);
        memcpy(new_data + offset, out_key, 32);
        sz["SYMKEY"] = offset + 32;
        char* nd = data["SYMKEY"];
        data["SYMKEY"] = new_data;
        delete[] nd;
        delete[] out_key;
    }
    return true;
}


void CryptFile::edit(std::string stream, char* content, int content_size, std::string user) {
    auto it = data.find(stream);
    if (it == data.end())
        throw std::runtime_error("Stream doesn't exist");
    if (user_rights.count(user) == 1) {
        //        *out_len = orig_sz[stream];
        uint8_t out_key[32];
        kuz_key_t sk;
        char* out_data;
        int offset = 0;
        //find key for file
        for (auto it = user_rights.begin(); it != user_rights.end(); it++) {
            if (it->first == "admin")
                continue;
            if (it->first != user) {
                offset += 32;
                continue;
            }
            kuz_set_decrypt_key(&sk, user_table[it->first].key);
            memcpy(out_key, data["SYMKEY"] + offset, 32);
            kuz_decrypt_block(&sk, out_key);
            kuz_decrypt_block(&sk, out_key + 16);
            //            std::cout << "Filekey: ";
            //            for (int i=0; i<32; i++){
            //                std::cout << std::hex << (unsigned int)(out_key[i] & 0xFF);
            //            }
            //            std::cout << std::endl;
            kuz_set_encrypt_key(&sk, out_key);
            out_data = kuz_encrypt_ctr(&sk, content, content_size);
            data[stream] = out_data;
            sz[stream] = content_size;
        }
    }
}


void CryptFile::add_stream_to_file(std::string, char*, int) {
    std::runtime_error("not implemented for CryptFile");
}


//void CryptFile::set_access(std::string user_name, access_rights){

//}

//    char* CryptFile::get_data(std::string stream, std::string user, const uint8_t &key[32], int *out_len) {
char* CryptFile::get_data(std::string stream, std::string user, int* out_len) {
    //    Map<std::string, std::string>::Iterator it = data.find(stream);
    auto it = data.find(stream);
    if (it == data.end())
        throw std::runtime_error("Stream doesn't exist");
    if (user_rights.count(user) == 1) {
        //        *out_len = orig_sz[stream];
        *out_len = sz[stream];
        uint8_t out_key[32];
        kuz_key_t sk;
        char* out_data = nullptr;
        int offset = 0;
        //find key for file
        for (auto it = user_rights.begin(); it != user_rights.end(); it++) {
            if (it->first == "admin")
                continue;
            if (it->first != user) {
                offset += 32;
                continue;
            }
            kuz_set_decrypt_key(&sk, user_table[it->first].key);
            memcpy(out_key, data["SYMKEY"] + offset, 32);
            kuz_decrypt_block(&sk, out_key);
            kuz_decrypt_block(&sk, out_key + 16);
            std::cout << "Filekey: ";
            for (int i = 0; i < 32; i++) {
                std::cout << std::hex << (unsigned int)(out_key[i] & 0xFF);
            }
            std::cout << std::endl;
            kuz_set_encrypt_key(&sk, out_key);
            out_data = kuz_encrypt_ctr(&sk, data[stream], sz[stream]);
        }
        return out_data;
        //    return it.get_value();
    }
    else {
        *out_len = -1;
        return nullptr;
    }
}

//// Encoder uuencoder logic
//// @method: encodes the contents of a file using uuencoding
//// @params: from - text to be encoded, to - output stream to store encoded text
//// @return: string of encoded text
//// from https://github.com/eastene/Uuencoder/blob/master/Uuencoder/Uuencoder/Encoder.cpp
//void CryptFile::uuencoder(std::iostream &from, std::iostream &to) {
//    // pulls 3 characters from stream at a time (3-bytes)
//    std::string chunk;
//    // number of bytes in the line
////    int byte_count = 0;
//    // 4-byte encoding
//    std::string bytes ("    ");
//    // a single encoded line
//    std::string line;

//    while(getline(from, chunk)){
//        // index of current spot in chunk
//        int c = 0;
//        int c_len = chunk.length();
////        _DPRINT("chunk length", chunk.length());
//        while (c_len - c - 45 >= 0){
////            _DPRINT("writing line", c / 45);
//            to << "M";
//            for (int i = c; i < c + 45; i += 3){
//                bytes[0] = (char) ((chunk[i] & 0b11111100) >> 2) + 32;
//                bytes[1] = (char) (((chunk[i] & 0b00000011) << 4) | ((chunk[i+1] & 0b11110000) >> 4)) + 32;
//                bytes[2] = (char) (((chunk[i+1] & 0b00001111) << 2) | ((chunk[i+2] & 0b11000000) >> 6)) + 32;
//                bytes[3] = (char) (chunk[i+2] & 0b00111111) + 32;

//                to << bytes;
//            }
//            to << "\n";
//            c += 45;
//        }

//        if (c_len - c > 0){
//            if ((chunk.length() - c) % 3 == 1){
//                chunk += "00";
//                c_len += 2;
//            }else if ((chunk.length() - c) % 3 == 2){
//                chunk += "0";
//                c_len += 1;
//            }

//            int size = (c_len - c);

//            to << (char) (size + 32);
//            for (int i = c; i < c + size; i += 3){
////                _DPRINT("iterator", i);
//                bytes[0] = (char) ((chunk[i] & 0b11111100) >> 2) + 32;
//                bytes[1] = (char) (((chunk[i] & 0b00000011) << 4) | ((chunk[i+1] & 0b11110000) >> 4)) + 32;
//                bytes[2] = (char) (((chunk[i+1] & 0b00001111) << 2) | ((chunk[i+2] & 0b11000000) >> 6)) + 32;
//                bytes[3] = (char) (chunk[i+2] & 0b00111111) + 32;

//                to << bytes;
//            }

//            to << "\n";
//        }
//    }
//}

//// Decoder uudecoder logic
//// @method: decodes text encoded with uuencoding
//// @params: contents - encoded text
//// @return: decoded string
//// from https://github.com/eastene/Uuencoder/blob/master/Uuencoder/Uuencoder/Decoder.cpp
//void CryptFile::uudecoder(std::iostream &from, std::iostream &to) {
//    // size of the line
//    int size;
//    // 3 decoded letters from 4 encoded bytes
//    std::string let("   ");
//    // current line of encoded text
//    std::string line;

//    // get stream size
//    from.seekp(0, std::ios::end);
//    int char_count = from.tellp();
//    from.seekp(0, std::ios::beg);

//    while (getline(from, line)){
//        // get and convert size
//        size = line[0];

//        if (size == '`'){
//            break;
//        }

//        size = size - 32;
////        _DPRINT("size", size);

//        for (int i = 1; i < line.length(); i += 4){
//            let[0] = (((line[i] - 32) & 0b00111111) << 2) | (((line[i+1] - 32) & 0b00110000) >> 4);
//            let[1] = (((line[i+1] - 32) & 0b00001111) << 4) | (((line[i+2] - 32) & 0b00111100) >> 2);
//            let[2] = (((line[i+2] - 32) & 0b00000011) << 6) | (((line[i+3] - 32) & 0b00111111));
////            _DPRINT("letters", let);

//            // remove padding 0s if needed
//            if (i > line.length() - 5){
//                if (size % 3 == 1){
//                    to << let[0];
//                }else if (size % 3 == 2){
//                    to << let[0] << let[1];
//                }else{
//                    to << let;
//                }
//            }
//            else{
//                to << let;
//            }
//        }
//    }
//}


