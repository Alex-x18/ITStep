#pragma once
#include "testing_system.h"
#include "config.h"
#include "module.h"
#include "user_repos.h"
#include "test_repos.h"
#include "test_result_repos.h"
#include "test_interrupt_repos.h"
namespace testing_system {
	class user_module : public module {
		public:
			user_module(const user_module&) = delete;
			user_module(user_module&&) = delete;
			void start_testing();
			void view_test_results();
			void continue_interrupted_test();
			void listen() noexcept override;
			void set_config(config* cfg) noexcept override;
			void free() noexcept override;
			void operator=(const user_module&) = delete;
			void operator=(user_module&&) = delete;
			static user_module& get() noexcept;
		private:
			void start_test_with(test& interrupted_test, test_interrupt_info* interrupted_info = nullptr);
			user_module() = default;
			config* cfg = nullptr;
	};
}