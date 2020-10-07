#pragma once
#include "testing_system.h"
#include "repos.h"
namespace testing_system {
	class question final : public serializable {
		friend class test;
		friend std::ostream& operator<<(std::ostream& os, const question& q);
		public:
			question() = default;
			question(const std::string& title, size_t correct_variant, const std::vector<std::string>& variants) noexcept:
				title(title),
				variants(variants),
				corrent_variant(correct_variant) {}
			std::string title;
			size_t corrent_variant = 0;
			std::vector<std::string> variants;
			bool operator!=(const question& rhs) const noexcept;
			bool operator==(const question& rhs) const noexcept;
			serialize_data serialize() const override;
			void deserialize(serialize_data& info) override;		
	};
	MAKE_EXCEPTION(test_question_not_found_exception);
	class test final : public serializable {
		friend std::ostream& operator<<(std::ostream& os, const test& t);
		public:
			test() noexcept:
				i(0),
				score(0) {}
			test(const std::string& category, const std::string& name):
				category(category),
				name(name),
				i(0),
				score(0) {}
			const std::string& get_category() const noexcept;
			const std::string& get_name() const noexcept;
			question& get_question(size_t index);
			const question& get_question(size_t index) const;
			void add_question(question& question);
			void remove_question(size_t index) noexcept;
			void answer(size_t variant) noexcept;
			void reset() noexcept;
			size_t get_score() const noexcept;
			size_t get_rawscore() const noexcept;
			size_t get_high_rawscore() const noexcept;
			double get_pscore() const noexcept;
			void set_rawscore(size_t score) noexcept;
			size_t question_size() const noexcept;
			size_t get_current_question_index() const noexcept;
			void set_current_question_index(size_t i) noexcept;
			operator bool() const noexcept;
			bool operator==(const test& rhs) const noexcept;
			bool operator!=(const test& rhs) const noexcept;
			std::vector<question>::iterator begin() noexcept;
			std::vector<question>::iterator end() noexcept;
			serialize_data serialize() const override;
			void deserialize(serialize_data& info) override;
		private:
			size_t i;
			size_t score;
			std::string category;
			std::string name;
			std::vector<question> questions;
	};
}