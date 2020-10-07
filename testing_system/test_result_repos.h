#pragma once
#include "testing_system.h"
#include "repos.h"
#include "test_result.h"
namespace testing_system {
	class test_result_repos : public repos<test_result>, public categorizable<test_result> {
		public:
			test_result_repos(const std::string& filename): repos(filename) {}
			void load() override;
			void reload() override;
			void save() override;
			void insert_and_update(const test_result& elem) override;
			std::vector<test_result> fetch_by_cat(const std::string& category) const noexcept override;
			std::set<std::string> fetch_cats() const noexcept override;
		private:
			void base_load();
	};
}