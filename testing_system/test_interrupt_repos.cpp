#include "test_interrupt_repos.h"
namespace testing_system {
	void test_interrupt_repos::load() {
		if (!loaded) {
			base_load();
			loaded = true;
		}
	}
	void test_interrupt_repos::reload() {
		if (loaded) {
			release();
			base_load();
		}
	}
	void test_interrupt_repos::save() {
		std::fstream fs(filename, std::ios::in | std::ios::out | std::ios::trunc);
		if (fs.is_open()) {
			for (auto& ti : *this) {
				fs << "$kv" << '\n';
				fs << "$question_index" << '\n';
				fs << ti.question_index << '\n';
				fs << "$test_name" << '\n';
				fs << ti.test_name << '\n';
				fs << "$test_cat" << '\n';
				fs << ti.test_cat << '\n';
				fs << "$user_score" << '\n';
				fs << ti.user_score << '\n';
			}
			fs.close();
		}
	}
	std::set<std::string> test_interrupt_repos::fetch_cats() const noexcept {
		std::set<std::string> cats;
		for (auto& ti : *this) {
			cats.insert(ti.test_cat);
		}
		return cats;
	}
	std::vector<test_interrupt_info> test_interrupt_repos::fetch_by_cat(const std::string& cat) const noexcept {
		std::vector<test_interrupt_info> test_interrupts;
		for (auto& ti : *this) {
			test_interrupts.push_back(ti);
		}
		return test_interrupts;
	}
	void test_interrupt_repos::base_load() {
		std::fstream fs(filename, std::ios::in | std::ios::out);
		if (fs.is_open()) {
			std::string line;
			test_interrupt_info* tresult = nullptr;
			while (std::getline(fs, line)) {
				if (line == "$kv") {
					if (tresult != nullptr) {
						insert(*tresult);
						delete tresult;
					}
					tresult = new test_interrupt_info;
					continue;
				}
				if (tresult != nullptr) {
					if (line == "$question_index") {
						std::getline(fs, line);
						tresult->question_index = static_cast<size_t>(std::stoi(line));
						continue;
					}
					if (line == "$test_name") {
						std::getline(fs, line);
						tresult->test_name = line;
						continue;
					}
					if (line == "$test_cat") {
						std::getline(fs, line);
						tresult->test_cat = line;
						continue;
					}
					if (line == "$user_score") {
						std::getline(fs, line);
						try {
							tresult->user_score = static_cast<size_t>(std::stoi(line));
						} catch (...) {
							delete tresult;
							tresult = nullptr;
						}
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
}