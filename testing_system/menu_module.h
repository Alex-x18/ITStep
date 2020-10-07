#pragma once
#include "testing_system.h"
#include "config.h"
#include "module.h"
#include "user.h"
namespace testing_system {
	class menu_module : public module {
		public:
			using action = int;
			using callback = std::function<void()>;
		private:
			struct menu_item {
				action n = 0;
				callback cb;
				std::string content;
			};
		public:
			menu_module(const menu_module&) = delete;
			menu_module(menu_module&&) = delete;
			void render();
			void set_config(config* cfg) noexcept override;
			void await();
			void add_item(action n, callback cb, const std::string& content, user_access_level ual);
			void add_item(action n, callback cb, const std::string& content);
			void listen() noexcept override;
			void free() noexcept override;
			void operator=(const menu_module&) = delete;
			void operator=(menu_module&&) = delete;
			static menu_module& get() noexcept;
		private:
			std::multimap<user_access_level, menu_item> menu_items;
			std::map<action, menu_item> default_menu_items;
			menu_module() = default;
			config* cfg = nullptr;
	};
}