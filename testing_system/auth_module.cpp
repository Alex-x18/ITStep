#include "auth_module.h"
namespace testing_system {
	void auth_module::auth() {
		std::string surname;
		std::string name;
		std::string fathers_name;
		std::string phone;
		std::string login;
		std::string password;
		std::string verified_password;
		user_access_level ual {};
		std::cout << "[type 0 to interrupt]\n";
		using namespace misc;
		try {
			interruptable_input("Name > ", name);
			interruptable_input("Surname > ", surname);
			interruptable_input("Father's name > ", fathers_name);
			interruptable_input("Phone > ", phone);
			interruptable_input("Login > ", login);
			if (cfg->urepos->exists([&login](const user& u) -> bool {
				return u.get_login() == login;
			})) {
				std::cout << "> User already exists\n";
				return;
			}
			interruptable_input("Password > ", password);
			do {
				interruptable_input("Re-enter password > ", verified_password);
				if (password == verified_password) {
					break;
				} else {
					std::cout << "> Incorrect password. Please try again\n";
				}
			} while (true);
			if (cfg->urepos->is_empty()) {
				ual = user_access_level::ADMIN;
			}
			cfg->urepos->insert_and_update(user(ual, surname, name, fathers_name, phone, login, password));
		} catch (interrupt_throw_type&) {
			return;
		}
	}
	void auth_module::login() {
		std::string login;
		std::string password;
		using namespace misc;
		input("Login > ", login);
		input("Password > ", password);
		try {
			auto& result = cfg->urepos->fetch([&login, &password](user& u) -> bool {
				return u.get_login() == login && u.get_password() == password;
			});
			cfg->current_user.reset(new user(result));
			std::cout << "> Successfully logged in. Hello, " << cfg->current_user->get_name() << '\n';
		} catch (fetch_exception&) {
			std::cout << "> Username or password is incorrect\n";
		}
	}
	void auth_module::logout() noexcept {
		cfg->current_user.release();
	}
	void auth_module::set_config(config* cfg) noexcept {
		this->cfg = cfg;
	}
	void auth_module::listen() noexcept {
		cfg->listening = this;
	}
	void auth_module::free() noexcept {
		cfg->listening = nullptr;
	}
	auth_module& auth_module::get() noexcept {
		static auth_module instance;
		return instance;
	}
}