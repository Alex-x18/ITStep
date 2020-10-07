#include "menu_module.h"
namespace testing_system {
	void menu_module::render() {
		if (cfg->current_user == nullptr) {
			for (auto& item : default_menu_items) {
				std::cout << item.second.n << ". " << item.second.content << '\n';
			}
		} else {
			auto result = menu_items.equal_range(cfg->current_user->get_access_level());
			for (auto it = result.first, end = result.second; it != end; it++) {
				std::cout << it->second.n << ". " << it->second.content << '\n';
			}
		}
	}
	void menu_module::set_config(config* cfg) noexcept {
		this->cfg = cfg;
	}
	void menu_module::listen() noexcept {
		cfg->listening = this;
	}
	void menu_module::free() noexcept {
		cfg->listening = nullptr;
	}
	void menu_module::await() {
		std::string raw_act;
		{
			using namespace misc;
			input("> ", raw_act);
		}
		action act;
		try {
			act = std::stoi(raw_act);
		} catch (...) {
			std::cout << "> Invalid action\n";
			return;
		}
		if (cfg->current_user == nullptr) {
			auto result = default_menu_items.equal_range(act);
			for (auto it = result.first, end = result.second; it != end; it++) {
				it->second.cb();
			}
		} else {
			auto result = menu_items.equal_range(cfg->current_user->get_access_level());
			for (auto it = result.first, end = result.second; it != end; it++) {
				if (act == it->second.n) {
					it->second.cb();
				}
			}
		}
	}
	void menu_module::add_item(action n, callback cb, const std::string& content, user_access_level ual) {
		menu_items.emplace(ual, menu_item {n, cb, content});
	}
	void menu_module::add_item(action n, callback cb, const std::string& content) {
		default_menu_items.emplace(n, menu_item {n, cb, content});
	}
	menu_module& menu_module::get() noexcept {
		static menu_module instance;
		return instance;
	}
}