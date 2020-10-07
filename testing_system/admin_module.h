#pragma once
#include "testing_system.h"
#include "config.h"
#include "module.h"
namespace testing_system {
	class test;
	class admin_module : public module {
		public:
			admin_module(const admin_module&) = delete;
			admin_module(admin_module&&) = delete;
			void create_user();
			void remove_user();
			void modify_user();
			void add_test();
			void remove_test();
			void modify_test();
			void view_stats();
			void set_config(config* cfg) noexcept override;
			void free() noexcept override;
			void listen() noexcept override;
			void operator=(const admin_module&) = delete;
			void operator=(admin_module&&) = delete;
			static admin_module& get() noexcept;
		private:
			bool add_question(test& this_test);
			void remove_question(test& this_test);
			void edit_question(test& this_test);
			void show_userlist();
			void show_testlist(std::set<std::string>& cats);
			admin_module() = default;
			config* cfg = nullptr;
	};
}