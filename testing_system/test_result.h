#pragma once
#include "testing_system.h"
#include "repos.h"
namespace testing_system {
	class test_result : public serializable {
		public:
			test_result() = default;
			std::string test_category;
			std::string test_name;
			size_t user_score = 0;
			std::string user_login;
			serialize_data serialize() const override;
			void deserialize(serialize_data& info) override;
			bool operator==(const test_result& rhs) const noexcept;
	};
}