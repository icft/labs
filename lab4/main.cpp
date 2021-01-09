//#include <random>
#include <iostream>
#include "FileSystem.h"
#include "user.h"
#include <thread>

int main1() {
//	int myseed = 1234;

//	std::default_random_engine rng(myseed);
//	std::uniform_int_distribution<int> rng_dist(0, 255);
//	char mychar = static_cast<unsigned char>(rng_dist(rng));
//    std::cout << std::hex << mychar;

//    std::string dn="dir_name";
//    std::string un="user";
    std::string fn="dir_name/some_file";

    FileSystem fs;
    fs.create_user("user","123","admin");
//    fs.create_user(un,"123","user"); // should be error
//    fs.create_user(un,"12332","admin"); // should be error
    fs.create_user("user2","123","admin");
    fs.edit_users_password("user2", "654321");
    fs.delete_user("user2", "admin");
    fs.edit_users_password("user", "654321");

    if (fs.create_file(fn, "MAIN", "some data", 9, "user") == false)
        std::cout << "test1: Ok, can't create wrong file" << std::endl;
    if (fs.create_file("fn1", "MAIN", "some data",9, "admin") == true)
        std::cout << "test2: Ok, created correct file" << std::endl;
    if (fs.create_file("fn1_user", "MAIN", "some data",9, "user") == true)
        std::cout << "test3: Ok, created correct file" << std::endl;
//        std::cout << "Ok, can't create file for user in root" << std::endl;
    fs.add_info("fn1_user", "second","another data", 13, "user");
    if (fs.create_dir("dir1", "admin") == true)
        std::cout << "test4: Ok, created dir" << std::endl;
    if (fs.create_dir("admin/dir1", "admin") == false)
        std::cout << "test5: Ok" << std::endl;
    if (fs.create_dir("dir1_user", "user") == true)
        std::cout << "test6: Ok, created dir" << std::endl;
    if (fs.create_file("dir1_user/fn1", "MAIN", "some data", 9, "user") == true)
        std::cout << "test7: Ok, created correct file" << std::endl;
    if (fs.create_file("dir1_user/fn2", "MAIN", "some data",9, "user") == true)
        std::cout << "test8: Ok, created correct file" << std::endl;
    if (fs.add_info("dir1_user/fn2", "second", "data2", 5, "user") == true)
        std::cout << "test9: Ok, created correct file" << std::endl;

    fs.print_file_info("fn1","admin");
    fs.print_file_info("fn1_user","admin");
//    fs.print_fs();
    fs.ls("admin");
    fs.ls("");
    fs.ls("dir1");
    fs.ls("dir1_user");
    fs.print_access_rights("fn1_user");
    std::cout << "Fixing access rights: for 'user' remove exec\n";
    fs.edit_access_rights("fn1_user", "user", {true,true,false}, "user");
    fs.print_access_rights("fn1_user");
    std::cout << "Fixing access rights: for 'admin' remove write\n";
    fs.edit_access_rights("fn1_user", "admin", {true,false,true}, "user");
    fs.print_access_rights("fn1_user");
    fs.print_access_rights("dir1_user/fn2");
    std::cout << "\nChecking fs.read_dir(\"dir1_user\", \"admin\")\n";
    auto v = fs.read_dir("dir1_user", "admin");
//    std::cout << v.size();
    for (auto it=v.cbegin(); it != v.cend(); it++)
        std::cout << it->data() << std::endl;
    if (fs.move("fn1_user", "dir1_user","user") == false)
        std::cout << "test10: cant move file by user"<< std::endl;
    fs.ls("dir1_user");
    if (fs.move("fn1_user", "dir1_user","admin") == true)
        std::cout << "test11: moved file by admin"<< std::endl;
    fs.ls("dir1_user");
    if (fs.move("dir1_user/fn1_user", "","admin") == true)
        std::cout << "test12: moved file by admin"<< std::endl;
    fs.ls("dir1_user");
    fs.ls("");

    if (fs.copy_file("fn1_user", "fn2_user", "user") == true)
        std::cout << "test13: copy file"<< std::endl;
    fs.ls("");
    fs.print_file_info("fn2_user", "admin");
    if (fs.copy_dir("dir1_user", "dir1_user_copy", "user"))
        std::cout << "test14:ok, copy dir"<< std::endl;
    fs.ls("");
    if (fs.copy_file("fn2_user", "dir1_user_copy/fn2_user_copy", "user"))
        std::cout << "test15:ok, copy file"<< std::endl;
    fs.ls("dir1_user_copy");
    if (fs.rename("dir1_user_copy/fn2_user_copy", "fn2_user_rename", "user")==false)
        std::cout << "test16:ok, rename file fail. OK, no rights for user"<< std::endl;
    if (fs.rename("dir1_user_copy/fn2_user_copy", "fn2_user_rename", "admin")==true)
        std::cout << "test17:ok, rename file ok. OK, got rights for admin"<< std::endl;
    fs.ls("dir1_user_copy");
    fs.print_fs(false);
    fs.create_crypted_file("fn1_crypt","MAIN", "somedat",8,"user");
    fs.ls("");
    int out_len;
    char* data;
    data = fs.read_file("fn1_crypt", "SYMKEY", "user", &out_len);
    std::cout << "SYMKEY data: ("<< std::dec<< out_len<<") ";
    for (int i=0; i<out_len; i++){
        std::cout << std::hex << int(data[i]&0xFF);
    }
    std::cout << std::endl;
    data = fs.read_file("fn1_crypt", "MAIN", "user", &out_len);
    std::cout << "Crypted data: ("<< std::dec<< out_len<<") ";
    for (int i=0; i<out_len; i++){
        std::cout << std::hex << int(data[i]&0xFF);
    }
    std::cout << std::endl;
    data = fs.read_crypted_file("fn1_crypt", "MAIN", "user", &out_len);
    std::cout << "DeCrypted data: ("<< std::dec<< out_len<<") ";
    for (int i=0; i<out_len; i++){
        std::cout << std::hex << int(data[i]&0xFF);
    }
    std::cout << " = " << data << std::endl;
    fs.print_file_info("fn1_crypt","user");

    std::cout << "==================== SAVE =======================" << std::endl;
    fs.save_fs_state("save.fs");
    fs.load_fs_state("save.fs");
    std::cout << "==================== AFTER LOAD =======================" << std::endl;
    fs.ls("");
//    fs.print_file_info("fn1_crypt","user");

    // test for bug in del_recurs
//    fs.create_dir("ugnhuags","user");
//    fs.create_dir("ugnhuags/dir1","user");
//    fs.create_file("ugnhuags/file1","MAIN","cont", 5, "user");
//    fs.print_fs(false);
//    fs.del("ugnhuags","user");

//    fs.create_dir("bnuokgef", "user");
//    fs.create_dir("bnuokgef/pbwownck", "user");
//    fs.create_dir("ehuzaemc","user");
//    fn = "ehuzaemc";
//    auto dir_name = "bnuokgef/pbwownck";
//    std::cout << "user" << " FS_Readdir_n_copy_file of '/" << fn << "' to '/" << dir_name << "' ";
//    auto res = fs.copy(fn, dir_name, "user");
//    std::cout << ((res)?"OK":"FAIL") << std::endl;

//    std::cout << "user" << " deleted file '" << fn << "' ";
//    res = fs.del(dir_name, "user");
//    std::cout << ((res)?"OK":"FAIL") << std::endl;

//    std::cout.width(4);
//    std::cout.fill('0');
//    std::cout << std::uppercase << std::hex << 123 << std::endl;
//    printf("%04X\n", 123);
//    char mas[] = "123\0\0123";
//    std::string str = mas;
//    std::cout << mas << std::endl;
//    std::cout << str << std::endl;
//    std::cout << str.size() << std::endl;


//    fs.cd("dir1","admin");
//    fs.create_dir(dn, "admin");
//    fs.create_dir("dir2", "user");
//    fs.print_fs();
    return 0;
}



std::vector<std::string> user_names = {
    "user1",
    "user2",
    "user3",
    "user4"
};


void run_func(User* &u, int &count){
//void run_func(int count){
    u->run(count);
//    std::cout << count;
}


int main2() {
    FileSystem fs;
//    fs.create_user("user","123","admin");
//    User u("user", &fs);
//    u.run(200);
    std::thread* a[user_names.size()];
    User* u[user_names.size()];
    for (std::size_t i=0; i < user_names.size(); i++){
        auto &name = user_names[i];
        fs.create_user(name,"123","admin");
        u[i] = new User(name, &fs);
        int cnt = 30;
        a[i] = new std::thread(run_func, std::ref(u[i]), std::ref(cnt));
    }
    for (std::size_t i=0; i < user_names.size(); i++){
        a[i]->join();
    }
    fs.print_fs(false);
    for (std::size_t i=0; i < user_names.size(); i++){
        delete a[i];
        delete u[i];
    }
    return 0;
}

int main3() {
    FileSystem fs;
    fs.create_user("user","123","admin");
    User u("user", &fs);
    u.run(200);
    fs.print_fs(false);
    return 0;
}

int main() {
    return main2();
}
