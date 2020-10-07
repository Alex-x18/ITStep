#include "user_repos.h"
namespace testing_system {
	void user_repos::load() {
		if (!loaded) {
			base_load();
			loaded = true;
		}
	}
	void user_repos::reload() {
		if (loaded) {
			release();
			base_load();
		}
	}
	void user_repos::save() {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
		if (fs.is_open()) {
			for (auto user : *this) {
				user.encrypt();
				auto uinfo = user.serialize();
				fs.write(reinterpret_cast<char*>(uinfo.get()), uinfo.size());
			}
			fs.close();
		}
	}
	void user_repos::insert_and_update(const user& elem) {
		if (!loaded) {
			return;
		}
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
		if (fs.is_open()) {
			user copy = elem;
			copy.encrypt();
			auto uinfo = copy.serialize();
			fs.write(reinterpret_cast<char*>(uinfo.get()), uinfo.size());
			fs.close();
			insert(elem);
		}
	}
	void user_repos::base_load() {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::binary);
		if (fs.is_open()) {
			std::string str_data_size;
			size_t data_size = 0;
			auto last_valid_pos = fs.tellg();
			str_data_size.reserve(12);
			while (true) {
				char c = fs.get();
				if (c == '$') {
					fs.get();
					while (true) {
						c = fs.get();
						if (c >= '0' && c <= '9') {
							str_data_size.push_back(c);
						} else {
							break;
						}
					}
					if (str_data_size.size() == 0) {
						throw user_metadata_exception("Couldn't find data size of buffer");
					}
					data_size = static_cast<size_t>(std::stoi(str_data_size));
					str_data_size.clear();
					fs.seekg(last_valid_pos);
					serialize_data info(data_size);
					fs.read(info.get(), info.size());
					user u;
					u.deserialize(info);
					u.decrypt();
					insert(u);
					last_valid_pos = fs.tellg();
					continue;
				}
				break;
			}
		}
	}
}