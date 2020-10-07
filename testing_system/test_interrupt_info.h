#pragma once
#include "testing_system.h"
namespace testing_system {
	struct test_interrupt_info {
		size_t question_index {};
		std::string test_name {};
		std::string test_cat {};
		size_t user_score {};
		bool operator==(const test_interrupt_info& rhs) const noexcept {
			return rhs.question_index == question_index && rhs.test_name == test_name && rhs.test_cat == test_cat && rhs.user_score == user_score;
		}
	};
}