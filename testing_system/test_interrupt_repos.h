#pragma once
#include "testing_system.h"
#include "repos.h"
#include "test_interrupt_info.h"
namespace testing_system {
	class test_interrupt_repos : public repos<test_interrupt_info>, public categorizable<test_interrupt_info> {
		public:
			test_interrupt_repos(const std::string& login): repos(login) {}
			void load() override;
			void reload() override;
			void save() override;
			std::set<std::string> fetch_cats() const noexcept override;
			std::vector<test_interrupt_info> fetch_by_cat(const std::string& cat) const noexcept override;
		private:
			void base_load();
	};
}