#pragma once
#include "testing_system.h"
#include "repos.h"
#include "user.h"
namespace testing_system {
	MAKE_EXCEPTION(user_metadata_exception);
	class user_repos final : public repos<user> {
		public:
			user_repos(const std::string filename): repos(filename) {}
			void load() override;
			void reload() override;
			void save() override;
			void insert_and_update(const user& elem) override;
		private:
			void base_load();
	};
}