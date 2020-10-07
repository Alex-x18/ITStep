#pragma once
#include "testing_system.h"
#include "module.h"
namespace testing_system {
	class user;
	class user_repos;
	class test_repos;
	class test_result_repos;
	class test_interrupt_repos;
	struct config {
		std::unique_ptr<user> current_user {};
		std::unique_ptr<user_repos> urepos {};
		std::unique_ptr<test_repos> trepos {};
		std::unique_ptr<test_result_repos> trrepos {};
		module* listening {};
	};
}