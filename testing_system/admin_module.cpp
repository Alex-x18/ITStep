#include "admin_module.h"
#include "user_repos.h"
#include "test_repos.h"
#include "test_result_repos.h"
namespace testing_system {
	void admin_module::create_user() {
		std::string surname;
		std::string name;
		std::string fathers_name;
		std::string phone;
		std::string login;
		std::string password;
		std::string access_level;
		user_access_level ual {};
		std::cout << "[type 0 to interrupt]\n";
		using namespace misc;
		try {
			interruptable_input("Login > ", login);
			if (cfg->urepos->exists([&login](const user& u) -> bool {
				return u.get_login() == login;
			})) {
				std::cout << "> User already exists\n";
				return;
			}
			interruptable_input("Password > ", password);
			interruptable_input("Access level (admin, default) > ", access_level);
			if (access_level == "admin") {
				ual = user_access_level::ADMIN;
			} else {
				ual = user_access_level::DEFAULT;
			}
			interruptable_input("Name > ", name);
			interruptable_input("Surname > ", surname);
			interruptable_input("Father's name > ", fathers_name);
			interruptable_input("Phone > ", phone);
			cfg->urepos->insert_and_update(user(ual, surname, name, fathers_name, phone, login, password));
		} catch (interrupt_throw_type&) {
			return;
		}
	}
	void admin_module::remove_user() {
		using namespace misc;
		std::cout << "> Specify a user login to delete\n";
		show_userlist();
		std::cout << "[type 0 to interrupt]\n";
		std::string rm_login;
		try {
			interruptable_input("> ", rm_login);
		} catch (interrupt_throw_type&) {
			return;
		}
		if (rm_login == cfg->current_user->get_login()) {
			std::cout << "> Cannot delete this user, because it is current user with ADMIN access level\n";
			return;			
		}
		try {
			auto& result = cfg->urepos->fetch([&rm_login](user& elem) -> bool {
				return elem.get_login() == rm_login;
			});
			cfg->urepos->erase_and_update(result);
			std::remove(rm_login.c_str()); //remove all interrupted tests file
		} catch (fetch_exception&) {
			std::cout << "> User doesn't exist\n";
		}
	}
	void admin_module::show_userlist() {
		for (auto& u : *cfg->urepos) {
			std::cout << "> login: " << u.get_login() << " [" << u.get_surname() << " " << u.get_name() << "," << u.get_fathers_name() << " ] " << u.get_phone() << " $ UAL: " << u.get_access_level() <<'\n';
		}
	}
	void admin_module::modify_user() {
		std::cout << "Specify a user login to modify >\n";
		show_userlist();
		std::cout << "[type 0 to interrupt]\n";
		using namespace misc;
		std::string md_login;
		try {
			interruptable_input("> ", md_login);
			auto& result = cfg->urepos->fetch([&md_login](user& elem) -> bool {
				return elem.get_login() == md_login;
			});
			std::string new_surname;
			std::string new_name;
			std::string new_fathers_name;
			std::string new_phone;
			std::string new_login;
			std::string new_password;
			interruptable_input("New name > ", new_name);
			interruptable_input("New surname > ", new_surname);
			interruptable_input("New father's > ", new_fathers_name);
			interruptable_input("New phone > ", new_phone);
			interruptable_input("New login > ", new_login);
			if (cfg->urepos->exists([&new_login](const user& u) -> bool {
				return u.get_login() == new_login;
			})) {
				std::cout << "> Specified user login already exists; login skipped\n";
			}
			interruptable_input("New password > ", new_password);
		} catch (fetch_exception&) {
			std::cout << "> User doesn't exist\n";
		} catch (interrupt_throw_type&) {
			return;
		}
	}
	void admin_module::add_test() {
		using namespace misc;
		std::string cat;
		std::string name;
		try {
			interruptable_input("Test category > ", cat);
			interruptable_input("Test name > ", name);
		} catch (interrupt_throw_type&) {
			return;
		}
		try {
			cfg->trepos->fetch([&name, &cat](test& t) -> bool {
				return t.get_name() == name && t.get_category() == cat;
			});
			std::cout << "> Test with such name already exists\n";
			return;
		} catch (fetch_exception&) {}
		//???
		if (cat.size() == 0) {
			std::cout << "> Test must have a category\n";
			return;
		}
		//???
		if (name.size() == 0) {
			std::cout << "> Test must have a name\n";
			return;
		}
		test new_test(cat, name);
		std::cout << "[type 0 to stop]\n";
		while (add_question(new_test));
		//refer to operator bool()
		//update only if the test have at least 1 question
		if (new_test) {
			cfg->trepos->insert_and_update(new_test);
			std::cout << "> Done!\n";
		}
	}
	void admin_module::remove_test() {
		auto cats = cfg->trepos->fetch_cats();
		show_testlist(cats);
		std::string name;
		std::string cat;
		std::cout << "[type 0 to interrupt]\n";
		using namespace misc;
		try {
			interruptable_input("Test category > ", cat);
			if (cats.count(cat) == 0) {
				std::cout << "> No such category\n";
				return;
			}
			interruptable_input("Test name > ", name);
			try {
				auto& res = cfg->trepos->fetch([&name, &cat](test& t) -> bool {
					return t.get_category() == cat && t.get_name() == name;
				});
				cfg->trepos->erase_and_update(res);
			} catch (fetch_exception&) {
				std::cout << "> No such test\n";
			}
		} catch (interrupt_throw_type&) {
			return;
		}
	}
	void admin_module::modify_test() {
		auto cats = cfg->trepos->fetch_cats();
		show_testlist(cats);
		if (cats.empty()) {
			return;
		}
		std::string name;
		std::string cat;
		using namespace misc;
		try {
			interruptable_input("Test category > ", cat);
			if (cats.count(cat) == 0) {
				std::cout << "> No such category\n";
				return;
			}
			interruptable_input("Test name > ", name);
		} catch (interrupt_throw_type&) {
			return;
		}
		test* this_test = nullptr;
		try {
			this_test = &cfg->trepos->fetch([&name, &cat](test& t) -> bool {
				return t.get_category() == cat && t.get_name() == name;
			});
		} catch (fetch_exception&) {
			std::cout << "> No such test\n";
			return;
		}
		{
			//Test display
			size_t q_number = 1;
			for (auto& q : *this_test) {
				std::cout << "Question " << q_number++ << ".\n";
				std::cout << q;
			}
		}
		{
			//Test editor
			std::string action;
			do {
				input("Specify action [edit/remove/add/exit] > ", action);
				if (action == "edit") {
					edit_question(*this_test);
				} else if (action == "remove") {
					remove_question(*this_test);
				} else if (action == "add") {
					add_question(*this_test);
				} else {
					break;
				}
			} while (true);
			if (this_test->question_size() == 0) {
				cfg->trepos->erase_and_update(*this_test);
			} else {
				cfg->trepos->save();
			}
		}
	}
	void admin_module::view_stats() {
		auto cats = cfg->trrepos->fetch_cats();
		if (cats.size() == 0) {
			std::cout << "> No passed tests\n";
			return;
		}
		size_t all_passed = 0;
		for (auto& cat : cats) {
			auto test_results = cfg->trrepos->fetch_by_cat(cat);
			std::cout << "> " << cat << '\n';
			size_t total_score = 0;
			for (auto& tr : test_results) {
				total_score += tr.user_score;
			}
			size_t passed = test_results.size();
			std::cout << "\tPassed > " << passed << " with avarage score of " << static_cast<double>(total_score) / static_cast<double>(passed) << '\n';
			all_passed += passed;
		}
		std::cout << "\tTotal passed tests > " << all_passed << '\n';
	}
	bool admin_module::add_question(test& this_test) {
		using namespace misc;
		std::string title;
		size_t correct_variant;
		std::vector<std::string> variants;
		size_t variant_num = 1;
		try {
			interruptable_input("Title > ", title);
		} catch (interrupt_throw_type&) {
			return false;
		}
		//???
		if (title.size() == 0) {
			std::cout << "> Question must have a title\n";
			return true;
		}
		do {
			std::string variant;
			std::cout << variant_num++;
			try {
				interruptable_input(". variant > ", variant);
			} catch (interrupt_throw_type&) {
				break;
			}
			variants.emplace_back(variant);
		} while (true);
		if (variants.size() < 2) {
			std::cout << "> Question must have at least 2 questions\n";
			return true;
		}
		std::cout << "Enter correct variant [" << 1 << ", " << variants.size() << "] > ";
		std::cin >> correct_variant;
		question q(title, correct_variant - 1, variants);
		this_test.add_question(q);
		return true;
	}
	void admin_module::remove_question(test& this_test) {
		size_t q_index;
		std::cout << "Specify question number to remove > ";
		std::cin >> q_index;
		this_test.remove_question(q_index - 1);
	}
	void admin_module::edit_question(test& this_test) {
		size_t q_index;
		std::cout << "Specify question number to edit > ";
		std::cin >> q_index;
		q_index--;
		auto& q = this_test.get_question(q_index);
		using namespace misc;
		std::cout << "[type 0 to leave question unchanged]\n";
		for (auto& v : q.variants) {
			std::string edited_variant;
			std::cout << "Variant > " << v << '\n';
			try {
				interruptable_input("Edit variant >", edited_variant);
				v.assign(edited_variant);
			} catch (interrupt_throw_type&) {
				continue;
			}
		}
		std::cout << "[type 0 to stop]\n";
		do {
			std::string variant;
			try {
				interruptable_input("New variant > ", variant);
				q.variants.emplace_back(variant);
			} catch (interrupt_throw_type&) {
				break;
			}
		} while (true);
		size_t new_correct_variant;
		std::cout << "Specify new correct variant (0 to leave unchanged) > ";
		std::cin >> new_correct_variant;
		if (new_correct_variant != 0) {
			q.corrent_variant = new_correct_variant;
		}
	}
	void admin_module::show_testlist(std::set<std::string>& cats) {
		if (cats.size() == 0) {
			std::cout << "> No available tests\n";
			return;
		}
		for (auto& cat : cats) {
			auto tests = cfg->trepos->fetch_by_cat(cat);
			std::cout << "> " << cat << ": ";
			size_t total_score = 0;
			auto end = tests.end();
			end--;
			for (auto& t : tests) {
				std::cout << t.get_name();
				if (t != *end) {
					std::cout << ", ";
				}
			}
			std::cout << '\n';
		}
	}
	void admin_module::set_config(config* cfg) noexcept {
		this->cfg = cfg;
	}
	void admin_module::free() noexcept {
		this->cfg->listening = nullptr;
	}
	void admin_module::listen() noexcept {
		this->cfg->listening = this;
	}
	admin_module& admin_module::get() noexcept {
		static admin_module instance;
		return instance;
	}
}