#pragma once
#include "testing_system.h"
#include "repos.h"
#include "test.h"
namespace testing_system {
	MAKE_EXCEPTION(test_metadata_exception);
	class test_repos : public repos<test>, public categorizable<test> {
		public: 
			test_repos(const std::string filename): repos(filename) {}
			void load() override;
			void reload() override;
			void save() override;
			void insert_and_update(const test& elem) override;
			std::vector<test> fetch_by_cat(const std::string& cat) const noexcept override;
			std::set<std::string> fetch_cats() const noexcept override;
		private:
			void base_load();
	};
}