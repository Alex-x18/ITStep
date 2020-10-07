#include "user_module.h"
namespace testing_system {
	void user_module::start_testing() {
		auto cats = cfg->trepos->fetch_cats();
		{
			if (cats.size() == 0) {
				std::cout << "> No available tests\n";
				return;
			}
			std::cout << "> Available categores: ";
			auto end = cats.end();
			--end;
			for (auto& c : cats) {
				std::cout << c;
				if (*end != c) {
					std::cout << ", ";
				}
			}
			std::cout << '\n';
		}
		std::string cat;
		std::cout << "[type 0 to interrupt]\n";
		using namespace misc;
		try {
			interruptable_input("Category > ", cat);
		} catch (interrupt_throw_type&) {
			return;
		}
		auto tests = cfg->trepos->fetch_by_cat(cat);
		if (tests.empty()) {
			std::cout << "> No such category\n";
			return;
		}
		for (auto& t : tests) {
			std::cout << "> " << t.get_name() << '\n';
		}
		std::string name;
		try {
			interruptable_input("Test name > ", name);
		} catch (interrupt_throw_type&) {
			return;
		}
		test* this_test = nullptr;
		for (auto& t : tests) {
			if (t.get_name() == name) {
				this_test = &t;
			}
		}
		if (this_test == nullptr) {
			std::cout << "> No such test\n";
			return;
		}
		start_test_with(*this_test);
	}
	void user_module::start_test_with(test& this_test, test_interrupt_info* interrupted_info) {
		size_t user_variant;
		bool interrupted = false;
		std::cout << "[type 0 to interrupt the test and continue later]\n";
		do {
			std::cout << this_test;
			std::cout << "Variant > ";
			std::cin >> user_variant;
			if (user_variant == 0) {
				interrupted = true;
				break;
			}
			this_test.answer(user_variant - 1);
		} while (this_test);
		auto& user_login = cfg->current_user->get_login();
		auto& cat = this_test.get_category();
		auto& name = this_test.get_name();
		std::unique_ptr<test_interrupt_repos> repos(new test_interrupt_repos(user_login));
		repos->load();
		if (interrupted) {
			if (interrupted_info != nullptr) {
				repos->erase_and_update(*interrupted_info);
			}
			test_interrupt_info new_interrupt_info;
			new_interrupt_info.test_cat = cat;
			new_interrupt_info.test_name = name;
			new_interrupt_info.user_score = this_test.get_rawscore();
			new_interrupt_info.question_index = this_test.get_current_question_index();
			repos->insert_and_update(new_interrupt_info);
		} else {
			test_result result;
			result.user_score = this_test.get_score();
			result.test_category = cat;
			result.test_name = name;
			result.user_login = user_login;
			std::cout << "Your score > " << this_test.get_score() << "; You answered " << this_test.get_rawscore() << " of " << this_test.get_high_rawscore() << " (" << this_test.get_pscore() << "%)\n";
			cfg->trrepos->insert_and_update(result);
			if (interrupted_info != nullptr) {
				repos->erase_and_update(*interrupted_info);
				if (repos->is_empty()) {
					std::remove(repos->get_filename().c_str());
				}
			}
		}
		this_test.reset();
	}
	void user_module::view_test_results() {
		size_t n = 0;
		auto& this_login = cfg->current_user->get_login();
		for (auto& tr : *cfg->trrepos) {
			if (tr.user_login == this_login) {
				std::cout << ++n << ". > \n";
				std::cout << "\tCat > " << tr.test_category << '\n';
				std::cout << "\tTest > " << tr.test_name << '\n';
				std::cout << "\tScore > " << tr.user_score << " / 12" << '\n';
			}
		}
		if (n == 0) {
			std::cout << "> No passed tests\n";
		}
	}
	void user_module::continue_interrupted_test() {
		test_interrupt_repos repos(cfg->current_user->get_login());
		repos.load();
		{
			size_t interrupted_test_n = 0;
			for (auto& ti : repos) {
				std::cout << ++interrupted_test_n << ". \n";
				std::cout << "\tCategory > " << ti.test_cat << '\n';
				std::cout << "\tName > " << ti.test_name << '\n';
			}
			if (interrupted_test_n == 0) {
				std::cout << "> No interrupted tests\n";
				return;
			}
		}
		std::string name;
		std::string cat;
		using namespace misc;
		input("Test category > ", cat);
		input("Test name > ", name);
		try {
			auto& interrupt_info = repos.fetch([&cat, &name](test_interrupt_info& ti) -> bool {
				return ti.test_cat == cat && ti.test_name == name;
			});
			try {
				auto& pure_test = cfg->trepos->fetch([&cat, &name](test& t) -> bool {
					return t.get_category() == cat && t.get_name() == name;
				});
				pure_test.set_current_question_index(interrupt_info.question_index);
				pure_test.set_rawscore(interrupt_info.user_score);
				start_test_with(pure_test, &interrupt_info);
			} catch (fetch_exception&) {
				std::cout << "> Specified test does not exist or original copy was deleted\n";
				repos.erase_and_update(interrupt_info);
				if (repos.is_empty()) {
					std::remove(repos.get_filename().c_str());
				}
			}
		} catch (fetch_exception&) {
			std::cout << "> Specified test is not in the list of interrupted\n";
		}
	}
	void user_module::listen() noexcept {
		cfg->listening = this;
	}
	void user_module::set_config(config* cfg) noexcept {
		this->cfg = cfg;
	}
	void user_module::free() noexcept {
		cfg->listening = nullptr;
	}
	user_module& user_module::get() noexcept {
		static user_module instance;
		return instance;
	}
}