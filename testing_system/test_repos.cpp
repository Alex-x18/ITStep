#include "test_repos.h"
namespace testing_system {
	void test_repos::load() {
		if (!loaded) {
			base_load();
			loaded = true;
		}
	}
	void test_repos::reload() {
		if (loaded) {
			release();
			base_load();
			loaded = true;
		}
	}
	void test_repos::save() {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
		if (fs.is_open()) {
			for (auto& t : *this) {
				auto info = t.serialize();
				fs.write(info.get(), info.size());
			}
			fs.close();
		}
	}
	void test_repos::insert_and_update(const test& elem) {
		if (!loaded) {
			return;
		}
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
		if (fs.is_open()) {
			auto elem_copy = elem;
			auto info = elem_copy.serialize();
			fs.write(info.get(), info.size());
			insert(elem);
			fs.close();
		}
	}
	void test_repos::base_load() {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::binary);
		if (fs.is_open()) {
			std::string str_data_size;
			size_t data_size = 0;;
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
						throw test_metadata_exception("Couldn't find data size of buffer");
					}
					data_size = static_cast<size_t>(std::stoi(str_data_size));
					str_data_size.clear();
					fs.seekg(last_valid_pos);
					serialize_data info(data_size);
					fs.read(info.get(), info.size());
					test u;
					u.deserialize(info);
					insert(u);
					last_valid_pos = fs.tellg();
					continue;
				}
				break;
			}
			fs.close();
		}
	}
	std::vector<test> test_repos::fetch_by_cat(const std::string& cat) const noexcept {
		std::vector<test> result;
		for (auto& t : *this) {
			if (t.get_category() == cat) {
				result.push_back(t);
			}
		}
		return result;
	}
	std::set<std::string> test_repos::fetch_cats() const noexcept {
		std::set<std::string> result;
		for (auto& t : *this) {
			result.insert(t.get_category());
		}
		return result;
	}
}