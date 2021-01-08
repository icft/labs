#include "gtest/gtest.h"
#include "Map.h"
#include "File.h"
#include "Dir.h"
#include "Cryptfile.h"
#include "FileSystem.h"

TEST(Map, Size) {
    Map<int, int> m;
    m[1] = 1;
    if (m.size() == 1)
        SUCCEED();
    else
        FAIL();
    m[3] = 3;
    m[4] = 4;
    if (m.size() == 3)
        SUCCEED();
    else
        FAIL();
    m.remove(3);
    if (m.size() == 2)
        SUCCEED();
    else
        FAIL();
    m.remove(4);
    m.remove(1);
    if (m.size() == 0)
        SUCCEED();
    else
        FAIL();
}

TEST(Map, Empty) {
    Map<int, int> m;
    if (m.is_empty())
        SUCCEED();
    else
        FAIL();
    m[1] = 1;
    if (!m.is_empty())
        SUCCEED();
    else
        FAIL();
    m.remove(1);
    if (m.is_empty())
        SUCCEED();
    else
        FAIL();
}

TEST(Map, IndexingOperator) {
    Map<int, int> m;
    m[1] = 1;
    m[2] = 34;
    auto it = m.find(1);
    auto it2 = m.find(2);
    if (it != m.end())
        SUCCEED();
    else
        FAIL();
    if (it2 != m.end())
        SUCCEED();
    else
        FAIL();
    ASSERT_EQ(m[1], 1);
    ASSERT_EQ(m[2], 34);
}

TEST(Map, Find) {
    Map<int, int> m;
    m[1] = 1;
    m[56] = 2;
    m[5] = 3;
    Map<int, int>::Iterator it1 = m.find(1);
    Map<int, int>::Iterator it2 = m.find(56);
    Map<int, int>::Iterator it3 = m.find(5);
    Map<int, int>::Iterator it4 = m.find(2);
    if (it1 != m.end() && it2 != m.end() && it3 != m.end() && it4 == m.end())
        SUCCEED();
    else
        FAIL();
    Map<int, int>::ConstIterator it5 = m.find(1);
    Map<int, int>::ConstIterator it6 = m.find(56);
    Map<int, int>::ConstIterator it7 = m.find(5);
    Map<int, int>::ConstIterator it8 = m.find(3);
    if (it1 != m.cend() && it2 != m.cend() && it3 != m.cend() && it4 == m.cend())
        SUCCEED();
    else
        FAIL();
}

TEST(Map, Remove) {
    Map<int, int> m;
    m[1] = 1;
    m[56] = 2;
    m[5] = 3;
    int sz = m.size();
    auto it = m.find(1);
    m.remove(it);
    if (sz - 1 == m.size() && m.find(1) == m.end())
        SUCCEED();
    else
        FAIL();
    m.remove(56);
    if (sz - 2 == m.size() && m.find(56) == m.end())
        SUCCEED();
    else
        FAIL();
    }

TEST(Map, Begin) {
    Map<int, int> m;
    m[5] = 6;
    m[7] = 3;
    m[1] = 4;
    auto it = m.begin();
    if (it == m.find(1))
        SUCCEED();
    else
        FAIL();
    if (m.find(7) != m.begin())
        SUCCEED();
    else
        FAIL();
    if (m.find(5) != m.cbegin())
        SUCCEED();
    else
        FAIL();
    auto it1 = m.cbegin();
    if (it1 == m.find(1))
        SUCCEED();
    else
        FAIL();
}

TEST(Map, End) {
    Map<int, int> m;
    m[5] = 6;
    m[7] = 3;
    m[1] = 4;
    auto it = m.find(7);
    if (++it == m.end())
        SUCCEED();
    else
        FAIL();
    if (m.find(1) != m.end())
        SUCCEED();
    else
        FAIL();
    if (m.find(5) != m.cend())
        SUCCEED();
    else
        FAIL();
    if (++m.find(7) == m.end())
        SUCCEED();
    else
        FAIL();
}

TEST(Files, Create) {
    File f;
    std::string name = "name";
    std::string stream = "MAIN";
    std::string user = "user";
    std::string inf = "hello";
    if(f.create(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    int a;
    std::string c(f.get_data(stream, user, &a));
    ASSERT_EQ(c, inf);
    ASSERT_EQ(f.filename, name);
    if (f.can_read(user) && f.can_write(user) && f.can_exec_or_move(user))
        SUCCEED();
    else
        FAIL();
    if (f.can_write("admin") && f.can_exec_or_move("admin") && f.can_read("admin"))
        SUCCEED();
    else
        FAIL();
    EXPECT_THROW(f.create(name, stream, inf.c_str(), 6, user), std::runtime_error);
}

TEST(Files, Edit) {
    File f;
    std::string name = "name";
    std::string stream = "MAIN";
    std::string user = "user";
    std::string inf = "hello";
    if(f.create(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    std::string inf2 = "anything";
    std::string user2 = "user2";
    f.edit(stream, const_cast<char *>(inf2.c_str()), 8, user);
    int a;
    std::string c(f.get_data(stream, user, &a));
    ASSERT_EQ(c, inf2);
    EXPECT_ANY_THROW(f.edit("owmfiewnive", const_cast<char *>(inf2.c_str()), 8, user));
}

TEST(Files, Get) {
    File f;
    std::string name = "name";
    std::string stream = "MAIN";
    std::string user = "user";
    std::string inf = "hello";
    if(f.create(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    int a;
    std::string c(f.get_data(stream, user, &a));
    ASSERT_EQ(c, inf);
}

TEST(Files, Set_access) {
    File f;
    std::string name = "name";
    std::string stream = "MAIN";
    std::string user = "user";
    std::string inf = "hello";
    if(f.create(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    access_rights ac = {true, false, false};
    f.set_access("ic", ac);
    if (f.can_read("ic") && !f.can_write("ac") && !f.can_exec_or_move("ac"))
        SUCCEED();
    else
        FAIL();
}

TEST(Files, Rights) {
    File f;
    access_rights ac = {true, false, true};
    f.set_access("us1", ac);
    ASSERT_EQ(f.can_read("us1"), true);
    ASSERT_EQ(f.can_write("us1"), false);
    ASSERT_EQ(f.can_exec_or_move("us1"), true);
}

TEST(Files, is_dir) {
    File f;
    std::string name = "name";
    std::string stream = "MAIN";
    std::string user = "user";
    std::string inf = "hello";
    if(f.create(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    ASSERT_EQ(f.is_dir(), false);
}

TEST(Dirs, Constructor) {
    std::string name = "lib";
    std::string own = "user";
    Dir d(name, own);
    ASSERT_EQ(d.owner, own);
    ASSERT_EQ(d.is_dir(), true);
    ASSERT_EQ(d.filename, name);
    if (d.can_read(own) && d.can_write(own) && d.can_exec_or_move(own))
        SUCCEED();
    else
        FAIL();
    if (d.can_write("admin") && d.can_exec_or_move("admin") && d.can_read("admin"))
        SUCCEED();
    else
        FAIL();

}

TEST(Dirs, Add_file) {
    Dir d("lib", "ic");
    File *f = new File;
    std::string name = "name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    auto t = f->create(name, stream, inf.c_str(), 6, user);
    if(t)
        SUCCEED();
    else
        FAIL();
    if (d.add_file(f, user))
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Constructor) {
    FileSystem fs;
    auto t = fs.get_table();
    users s;
    s.password = "123";
    for (int i=0; i<32; i++)
        s.key[i] = i;
    if (t["admin"].password == "123")
        SUCCEED();
    else
        FAIL();
}

TEST(FS, CreateUser) {
    FileSystem fs;
    fs.create_user("us1", "1234", "admin");
    auto t = fs.get_table();
    if (t.find("us1") != t.end())
        SUCCEED();
    else
        FAIL();
    EXPECT_THROW(fs.create_user("us", "1", "us1"), std::runtime_error);
    EXPECT_THROW(fs.create_user("us1", "1234", "admin"), std::runtime_error);
}

TEST(FS, Edit_users_password) {
    FileSystem fs;
    fs.create_user("us1", "1234", "admin");
    EXPECT_THROW(fs.edit_users_password("us", "11313"), std::runtime_error);
    fs.edit_users_password("us1", "1313131311311");
    if (fs.get_table()["us1"].password == "1313131311311")
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Delete_users) {
    FileSystem fs;
    fs.create_user("us1", "1234", "admin");
    fs.create_user("ic", "1234fdfc", "admin");
    EXPECT_THROW(fs.delete_user("ic", "us1"), std::runtime_error);
    fs.delete_user("ic", "admin");
    auto t = fs.get_table();
    if (t.find("ic") == t.end())
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Create_file) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    File *f = new File;
    std::string name = "/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    auto t = fs.create_file(name, stream, inf.c_str(), 6, user);
    if (t)
        SUCCEED();
    else
        FAIL();
    int a;
    std::string c(fs.read_file(name, stream, user, &a));
    if (c == inf)
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Add_info) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    File *f = new File;
    std::string name = "/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    auto t = fs.create_file(name, stream, inf.c_str(), 6, user);
    if (t)
        SUCCEED();
    else
        FAIL();
    std::string stream1 = "anything";
    std::string inf1 = "world";
    auto t1 = fs.add_info(name, stream1, const_cast<char *>(inf1.c_str()), 5, user);
    if (t1)
        SUCCEED();
    else
        FAIL();
    int a, b;
    std::string c(fs.read_file(name, stream, user, &a));
    std::string d(fs.read_file(name, stream1, user, &b));
    if (c == inf && d == inf1)
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Edit) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    File *f = new File;
    std::string name = "/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    auto t = fs.create_file(name, stream, inf.c_str(), 6, user);
    if (t)
        SUCCEED();
    else
        FAIL();
    std::string inf1 = "world";
    t = fs.edit(name, stream, const_cast<char *>(inf1.c_str()), 5, user);
    if (t)
        SUCCEED();
    else
        FAIL();
    int a;
    std::string c(fs.read_file(name, stream, user, &a));
    if (c == inf1)
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Create_dir) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    fs.create_dir("/lib", "ic");
    if (fs.is_dir("/lib", "ic"))
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Copy_file) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    fs.create_dir("/lib", "ic");
    std::string name = "/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    if (fs.copy(name, "/lib", user))
        SUCCEED();
    else
        FAIL();
    int a;
    if (fs.read_file(name, stream, user, &a))
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Read_dir) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    fs.create_dir("/lib", "ic");
    std::string name = "/lib/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    name = "/lib/ss";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    std::vector<std::string> v = fs.read_dir("/lib", user);
    if (v[0] == "name" && v[1] == "ss")
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Copy_dir) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    fs.create_dir("/lib", "ic");
    std::string name = "/lib/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    name = "/lib/ss";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    fs.create_dir("/bin", user);
    if (fs.copy("/lib", "/bin", user))
        SUCCEED();
    else
        FAIL();
    std::vector<std::string> v = fs.read_dir("/bin", user);
    if (v[0] == "lib")
        SUCCEED();
    else
        FAIL();
    v = fs.read_dir("/bin/lib", user);
    if (v[0] == "name" && v[1] == "ss")
        SUCCEED();
    else
        FAIL();
    std::vector<std::string> v1 = fs.read_dir("/lib", user);
    if (!v1.empty())
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Del) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    fs.create_dir("/lib", "ic");
    std::string name = "/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    name = "/lib/ss";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    fs.del("/name", user);
    fs.del("/lib", user);
    std::vector<std::string> v = fs.read_dir("/lib", user);
    if (v.empty())
        SUCCEED();
    else
        FAIL();
    int a;
    char* c = fs.read_file("/name", stream, user, &a);
    if (!c)
        SUCCEED();
    else
        FAIL();
}

TEST(FS, Move) {
    FileSystem fs;
    fs.create_user("ic", "11111", "admin");
    fs.create_dir("/lib", "ic");
    std::string name = "/name";
    std::string stream = "MAIN";
    std::string user = "ic";
    std::string inf = "hello";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    name = "/lib/ss";
    if (fs.create_file(name, stream, inf.c_str(), 6, user))
        SUCCEED();
    else
        FAIL();
    if(fs.move("/name", "/lib", user))
        SUCCEED();
    else
        FAIL();
    int a;
    if (!fs.read_file("/name", stream, user, &a))
        SUCCEED();
    else
        FAIL();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

