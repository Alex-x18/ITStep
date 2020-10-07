#include "testing_system.h"
#include "auth_module.h"
#include "menu_module.h"
#include "user_module.h"
#include "admin_module.h"

namespace testing_system {
	namespace misc {
		void interruptable_input(const std::string& msg, std::string& param) {
			std::cout << msg;
			std::getline(std::cin >> std::ws, param);
			if (param == "0") {
				throw interrupt_throw_type {};
			}
		}
		void input(const std::string& msg, std::string& param) noexcept {
			std::cout << msg;
			std::getline(std::cin >> std::ws, param);
		}
		size_t filesize(const std::string& filename) {
			std::fstream fs(filename, std::ios::ate | std::ios::out);
			if (fs.is_open()) {
				size_t res = fs.tellg();
				fs.close();
				return res;
			}
			throw std::exception("Cannot open file");
		}
		const size_t default_sleep_duration = 2333;
		void clr_console() {
		#if defined _WIN64 || defined _WIN32
			std::system("cls");
		#else
			std::system("clear");
		#endif
		}
		void sleep(size_t ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
	}
}
using namespace testing_system;
class application {
	public:
		application(): 
			auth_mod(auth_module::get()),
			menu_mod(menu_module::get()),
			user_mod(user_module::get()),
			admin_mod(admin_module::get()) {
			static bool one_touch = false;
			if (!one_touch) {
				one_touch = true;
				return;
			}
			throw std::logic_error("application can only exist in one instance");
		};
		application(const application&) = delete;
		application(application&&) = delete;
		void start() {
			init();
			init_menu();
			menu_mod.listen();
			while (main_cfg.listening == &menu_mod) {
				menu_mod.render();
				menu_mod.await();
			}
		}
		void operator=(const application&) = delete;
		void operator=(application&&) = delete;
	private:
		void init() {
			main_cfg.urepos.reset(new user_repos("udata.bin"));
			main_cfg.trepos.reset(new test_repos("tdata.bin"));
			main_cfg.trrepos.reset(new test_result_repos("trdata"));
			main_cfg.urepos->load();
			main_cfg.trepos->load(); 
			main_cfg.trrepos->load();
			auth_mod.set_config(&main_cfg);
			menu_mod.set_config(&main_cfg);
			user_mod.set_config(&main_cfg);
			admin_mod.set_config(&main_cfg);
		}
		void init_menu() {
			auto exit_cb = [this]() -> void {
				menu_mod.free();
			};
			auto logout_cb = [this]() -> void {
				auth_mod.listen();
				auth_mod.logout();
				menu_mod.listen();
			};
			{
				//Anonymous
				menu_mod.add_item(1, [this]() -> void {
					auth_mod.listen();
					auth_mod.auth();
					menu_mod.listen();
				}, "Sign up");
				menu_mod.add_item(2, [this]() -> void {
					auth_mod.listen();
					auth_mod.login();
					menu_mod.listen();
				}, "Sign in");
				menu_mod.add_item(3, exit_cb, "Exit");
			}
			{
				//Default User
				const user_access_level ual = user_access_level::DEFAULT;
				menu_mod.add_item(1, [this]() -> void {
					user_mod.listen();
					user_mod.start_testing();
					menu_mod.listen();
				}, "Pass a test", ual);
				menu_mod.add_item(2, [this]() -> void {
					user_mod.listen();
					user_mod.continue_interrupted_test();
					menu_mod.listen();
				}, "Interrupted tests", ual);
				menu_mod.add_item(3, [this]() -> void {
					user_mod.listen();
					user_mod.view_test_results();
					menu_mod.listen();
				}, "View results", ual);
				menu_mod.add_item(4, logout_cb, "Logout", ual);
				menu_mod.add_item(5, exit_cb, "Exit", ual);
			}
			{
				//Admin
				const user_access_level ual = user_access_level::ADMIN;
				menu_mod.add_item(1, [this]() -> void {
					admin_mod.listen();
					admin_mod.create_user();
					menu_mod.listen();
				}, "Create user", ual);
				menu_mod.add_item(2, [this]() -> void {
					admin_mod.listen();
					admin_mod.remove_user();
					menu_mod.listen();
				}, "Remove user", ual);
				menu_mod.add_item(3, [this]() -> void {
					admin_mod.listen();
					admin_mod.modify_user();
					menu_mod.listen();
				}, "Modify user", ual);
				menu_mod.add_item(4, [this]() -> void {
					admin_mod.listen();
					admin_mod.view_stats();
					menu_mod.listen();
				}, "View stats", ual);
				menu_mod.add_item(5, [this]() -> void {
					admin_mod.listen();
					admin_mod.add_test();
					menu_mod.listen();
				}, "Add a test", ual);
				menu_mod.add_item(6, [this]() -> void {
					admin_mod.listen();
					admin_mod.remove_test();
					menu_mod.listen();
				}, "Remove a test", ual);
				menu_mod.add_item(7, [this]() -> void {
					admin_mod.listen();
					admin_mod.modify_test();
					menu_mod.listen();
				}, "Modify a test", ual);
				menu_mod.add_item(8, logout_cb, "Logout", ual);
				menu_mod.add_item(9, exit_cb, "Exit", ual);
			}
		}
		auth_module& auth_mod;
		menu_module& menu_mod;
		user_module& user_mod;
		admin_module& admin_mod;
		config main_cfg;
};
int main() {
	application app;
	app.start();
}