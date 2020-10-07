#pragma once
#include "testing_system.h"
#include "module.h"
#include "config.h"
#include "user_repos.h"
#include "test_repos.h"
namespace testing_system {
	class auth_module : public module {
		public:
			auth_module(const auth_module&) = delete;
			auth_module(auth_module&&) = delete;
			void auth();
			void login();
			void logout() noexcept;
			void set_config(config* cfg) noexcept override;
			void listen() noexcept override;
			void free() noexcept override;
			void operator=(const auth_module&) = delete;
			void operator=(auth_module&&) = delete;
			static auth_module& get() noexcept;
		private:
			auth_module() = default;
			config* cfg = nullptr;
	};
}