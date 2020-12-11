#include "FileSystem.h"

std::pair<std::vector<std::string>, std::string> FileSystem::parser(std::string name) {
	char sep = '/';
	std::vector<std::string> v;
	for (size_t a = 0, b = 0; a != name.npos; a = b)
		v.push_back(name.substr(a + (a != 0), (b = name.find(sep, a + 1)) - a - (a != 0)));
	std::pair<std::vector<std::string>, std::string> _pair;
	_pair.first = v;
	if (v.size() == 1)
		_pair.second = v[0];
	else
		_pair.second = v[v.size() - 1];
	return _pair;
}

Dir* FileSystem::HELP(std::pair<std::vector<std::string>, std::string> _pair, int cr) {
	if (_pair.first.size() == 1) {
		std::map<std::string, File*>::iterator it = root.find(_pair.second);
		if (it != root.end())
			if (cr == 1)
				throw std::exception("File already exist");
			else 
				throw std::exception("File doesn't exist");
		return nullptr;
	}
	std::map<std::string, File*>::iterator it = root.find(_pair.second);
	if (it == root.end())
		throw std::exception("Directory in the path doesn't exist");
	Dir* cur = dynamic_cast<Dir*>(root[_pair.first[0]]);
	for (int i = 1; i < _pair.first.size() - 1; i++) {
		std::map<std::string, File*>::iterator it = cur->elems.find(_pair.first[i]);
		if (it == cur->elems.end())
			throw std::exception("Directory in the path doesn't exist");
		Dir* cur = dynamic_cast<Dir*>(cur->elems[_pair.first[i]]);
	}
	std::map<std::string, File*>::iterator it = cur->elems.find(_pair.second);
	if (it != cur->elems.end())
		if (cr == 1)
			throw std::exception("This file already exist");
		else
			throw std::exception("File doesn't exist");
	else {
		return cur;
	}
}

void FileSystem::create_file(std::string name, std::string stream, std::string s, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 1);
	if (fl == nullptr) {
		File* f = new File;
		f->create(_pair.second, stream, s, own);
		root[_pair.second] = f;
	} else {
		File* f = new File;
		f->create(_pair.second, stream, s, own);
		fl->elems[_pair.second] = f;
	}
}

void FileSystem::print(std::string name, std::string stream, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		if (root[_pair.second]->can_read(own))
			std::cout << root[_pair.second]->get_data(stream);
		else
			throw std::exception("Access denied");
	}
	else {
		if (fl->elems[_pair.second]->can_read(own))
			std::cout << fl->elems[_pair.second]->get_data(stream);
		else
			throw std::exception("Access denied");
	}
}

void FileSystem::edit(std::string name, std::string stream, std::string s, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		if (root[_pair.second]->can_write(own))
			root[_pair.second]->edit(stream, s);
		else
			throw std::exception("Access denied");
	}
	else {
		if (fl->elems[_pair.second]->can_write(own))
			fl->elems[_pair.second]->edit(stream, s);
		else
			throw std::exception("Access denied");
	}
}

void FileSystem::print_file_info(std::string name, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		if (root[_pair.second]->can_read(own))
			std::cout << root[_pair.second];
		else
			throw std::exception("Access denied");
	}
	else {
		if (fl->elems[_pair.second]->can_read(own))
			std::cout << fl->elems[_pair.second];
		else
			throw std::exception("Access denied");
	}
}

void FileSystem::get_access_rights(std::string name, std::string user) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		access_rights ac = root[_pair.second]->rights[user];
		std::cout << "Can read-" << ac.can_read << "\n";
		std::cout << "Can write-" << ac.can_write << "\n";
		std::cout << "Can execute-" << ac.can_execute_or_move << "\n";
	}
	else {
		access_rights ac = fl->elems[_pair.second]->rights[user];
		std::cout << "Can read-" << ac.can_read << "\n";
		std::cout << "Can write-" << ac.can_write << "\n";
		std::cout << "Can execute-" << ac.can_execute_or_move << "\n";
	}
}

void FileSystem::edit_access_rights(std::string name, std::string user, access_rights ac, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		if (own == "admin" || own == root[_pair.second]->owner) {
			root[_pair.second]->rights[user] = ac;
		}
		else
			throw std::exception("Access denied");
	}
	else {
		if (own == "admin" || own == root[_pair.second]->owner) {
			fl->elems[_pair.second]->rights[user] = ac;
		}
		else
			throw std::exception("Access denied");
	}
}

void FileSystem::encrypt(std::string name, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 1);
	if (fl == nullptr) {
		CryptFile* cf = dynamic_cast<CryptFile*>(root[_pair.second]);
		cf->cipher_data[own] = name;
		root[_pair.second]->data[;
	}
	else {
		CryptFile* cf = dynamic_cast<CryptFile*>(fl->elems[_pair.second]);
		cf->cipher_data[own] = name;
		root[_pair.second]->data["symkey"] = "";
	}
}

void FileSystem::decrypt(std::string name, std::string own) {

}

uint8_t* FileSystem::generate_key()
{
	return nullptr;
}

void FileSystem::create_user(std::string user, std::string password, std::string own) {
	if (own != "admin")
		throw std::exception("Access denied");
	users s;
	s.username = user;
	s.password = password;
	//добавить генерацию ключа
	table.push_back(s);
}

void FileSystem::edit_users_password(std::string name, std::string password) {
	std::vector<users>::iterator it = table.begin();
	for (; it != table.end(); it++)
		if (it->username == name) {
			it->password = password;
			break;
		}
	if (it == table.end())
		throw std::exception("User doesn't exist");
}

void FileSystem::delete_user(std::string name, std::string own) {
	if (own != "admin")
		throw std::exception("Access denied");
	std::vector<users>::iterator it = table.begin();
	for (; it != table.end(); it++)
		if (it->username == name) {
			break;
		}
	if (it == table.end())
		throw std::exception("User doesn't exist");
	table.erase(it);
}

void FileSystem::print_fs() {
	std::map<std::string, File*>::iterator it = root.begin();
	for (; it != root.end(); it++) {
		std::cout << it->first;
		if (it->second->is_dir()) {
			Dir* d = dynamic_cast<Dir*>(it->second);
			print_help(d, "  ");
		}
	}
}

void FileSystem::print_help(Dir* d, std::string s) {
	std::map<std::string, File*>::iterator it = d->elems.begin();
	for (; it != d->elems.end(); it++) {
		std::cout << s + it->first << "\n";
		if (it->second->is_dir()) {
			Dir* p = dynamic_cast<Dir*>(it->second);
			print_help(p, s + "  ");
		}
	}
}

void FileSystem::print_statistics() {
	int amount = 0, size = 0;
	std::map<std::string, File*>::iterator it = root.begin();
	for (; it != root.end(); it++) {

	}
}

int FileSystem::help_statistics(Dir* d) {

}

void FileSystem::create_dir(std::string name, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 1);
	if (fl == nullptr) {
		File* f = new File;
		f->create_dir(name, own);
		root[_pair.second] = f;
	}
	else {
		File* f = new File;
		f->create_dir(name, own);
		fl->elems[_pair.second] = f;
	}
}

void FileSystem::read_dir(std::string name, std::string own) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		Dir* d = dynamic_cast<Dir*>(root[_pair.second]);
		d->print();
	}
	else {
		Dir* d = dynamic_cast<Dir*>(fl->elems[_pair.second]);
		d->print();
	}
}

void FileSystem::copy_dir(std::string name, std::string where_name, std::string own) {
	if (name == where_name)
		throw std::exception("The file paths are the same");
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		if (!root[_pair.second]->rights[own].can_execute_or_move)
			throw std::exception("Access denied");
	}
	else {
		if(!root[_pair.second]->rights[own].can_execute_or_move)
			throw std::exception("Access denied");
	}
}

void FileSystem::move_dir(std::string name, std::string own) {
}

void FileSystem::del_dir(std::string) {
}

void FileSystem::add_rights(std::string name, std::string user, access_rights ac) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr)
		root[name]->add_access(user, ac);
	else
		fl->elems[name]->add_access(user, ac);
}

void FileSystem::rename(std::string name) {
	std::pair<std::vector<std::string>, std::string> _pair = parser(name);
	Dir* fl = HELP(_pair, 0);
	if (fl == nullptr) {
		std::map<std::string, File*>::iterator it = root.find(_pair.second);
		File* f = it->second;
		root.erase(it);
		root[_pair.second] = f;
	}
	else {
		std::map<std::string, File*>::iterator it = fl->elems.find(_pair.second);
		File* f = it->second;
		fl->elems.erase(it);
		fl->elems[_pair.second] = f;
	}
}
