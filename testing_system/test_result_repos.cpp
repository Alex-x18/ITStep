#include "test_result_repos.h"
namespace testing_system {
	void test_result_repos::load() {
		if (!loaded) {
			base_load();
			loaded = true;
		}
	}
	void test_result_repos::reload() {
		if (loaded) {
			release();
			base_load();
		}
	}
	void test_result_repos::save() {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::trunc);
		if (fs.is_open()) {
			for (auto& tr : *this) {
				fs << "$kv" << '\n';
				fs << "$test_category" << '\n';
				fs << tr.test_category << '\n';
				fs << "$test_name" << '\n';
				fs << tr.test_name << '\n';
				fs << "$score" << '\n';
				fs << tr.user_score << '\n';
				fs << "$user_login" << '\n';
				fs << tr.user_login << '\n';
			}
			fs.close();
		}
	}
	void test_result_repos::insert_and_update(const test_result& elem) {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::app);
		if (fs.is_open()) {
			fs << "$kv" << '\n';
			fs << "$test_category" << '\n';
			fs << elem.test_category << '\n';
			fs << "$test_name" << '\n';
			fs << elem.test_name << '\n';
			fs << "$score" << '\n';
			fs << elem.user_score << '\n';
			fs << "$user_login" << '\n';
			fs << elem.user_login << '\n';
			insert(elem);
			fs.close();
		}
	}
	void test_result_repos::base_load() {
		std::fstream fs(filename, std::ios::in | std::ios::out);
		if (fs.is_open()) {
			std::string line;
			test_result* tresult = nullptr;
			while (std::getline(fs, line)) {
				if (line == "$kv") {
					if (tresult != nullptr) {
						insert(*tresult);
						delete tresult;
					}
					tresult = new test_result;
					continue;
				}
				if (tresult != nullptr) {
					if (line == "$test_category") {
						std::getline(fs, line);
						tresult->test_category = line;
						continue;
					}
					if (line == "$test_name") {
						std::getline(fs, line);
						tresult->test_name = line;
						continue;
					}
					if (line == "$score") {
						std::getline(fs, line);
						try {
							tresult->user_score = static_cast<size_t>(std::stoi(line));
						} catch (...) {
							delete tresult;
							tresult = nullptr;
						}
						continue;
					}
					if (line == "$user_login") {
						std::getline(fs, line);
						tresult->user_login = line;
						continue;
					}
				}
			}
			if (tresult != nullptr) {
				insert(*tresult);
				delete tresult;
			}
		}
	}
	std::vector<test_result> test_result_repos::fetch_by_cat(const std::string& cat) const noexcept {
		std::vector<test_result> result;
		for (auto& tr : *this) {
			if (tr.test_category == cat) {
				result.push_back(tr);
			}
		}
		return result;
	}
	std::set<std::string> test_result_repos::fetch_cats() const noexcept {
		std::set<std::string> result;
		for (auto& tr : *this) {
			result.insert(tr.test_category);
		}
		return result;
	}
}