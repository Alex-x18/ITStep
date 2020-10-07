#include "test.h"
#include "ext.h"
namespace testing_system {
	std::ostream& operator<<(std::ostream& os, const question& q) {
		std::cout << "> " << q.title << '\n';
		size_t counter = 1;
		for (auto& v : q.variants) {
			std::cout << '\t' << counter++ << ". " << v << '\n';
		}
		return os;
	}
	serialize_data question::serialize() const {
		std::string data = "$";
		std::string correct_variant_str = std::to_string(corrent_variant);
		{
			//metadata
			data.append(":" + std::to_string(title.size()));
			data.append(":" + std::to_string(correct_variant_str.size()));
			for (auto& v : variants) {
				data.append(":" + std::to_string(v.size()));
			}
			data.push_back(';');
		}
		{
			//rawdata
			data.append(title);
			data.append(correct_variant_str);
			for (auto& v : variants) {
				data.append(v);
			}
		}
		auto data_size = data.size();
		{
			auto meta_info_size = (":" + std::to_string(data.size())).size();
			data_size += meta_info_size;
		}
		serialize_data info(data_size);
		data.insert(1, ":" + std::to_string(data_size));
		std::memmove(info.get(), data.c_str(), data_size);
		return info;
	}
	void question::deserialize(serialize_data& info) {
		std::string data;
		std::vector<std::string> metadata;
		{
			std::string rawdata;
			rawdata.assign(info.get(), info.size());
			auto split = stdstring_ext::split_first(rawdata, ";");
			if (split.size() != 2) {
				throw deserialize_exception("data cannot be deserialized due to incorrect syntax");
			}
			metadata = stdstring_ext::split(split[0], ":");
			data = split[1];
		}
		try {
			size_t main_shift_pos = 0;
			auto __title_size = static_cast<size_t>(std::stoi(metadata.at(2)));
			auto __correct_variant_size = static_cast<size_t>(std::stoi(metadata.at(3)));
			{
				title = data.substr(main_shift_pos, __title_size);
				main_shift_pos += __title_size;
				corrent_variant = std::stoi(data.substr(main_shift_pos, __correct_variant_size));
				main_shift_pos += __correct_variant_size;
			}
			for (size_t i = 4; i < metadata.size(); i++) {
				auto __variant_size = static_cast<size_t>(std::stoi(metadata.at(i)));
				variants.emplace_back(data.substr(main_shift_pos, __variant_size));
				main_shift_pos += __variant_size;
			}
		} catch (...) {
			throw deserialize_exception("cannot deserialize data due to incorrect syntax of metadata");
		}
	}
	bool question::operator!=(const question& rhs) const noexcept {
		return rhs.title == title && rhs.corrent_variant == corrent_variant && rhs.variants == variants;
	}
	bool question::operator==(const question& rhs) const noexcept {
		return !operator!=(rhs);
	}
	std::ostream& operator<<(std::ostream& os, const test& t) {
		os << t.questions[t.i];
		return os;
	}
	const std::string& test::get_category() const noexcept {
		return category;
	}
	const std::string& test::get_name() const noexcept {
		return name;
	}
	void test::add_question(question& question) {
		questions.push_back(question);
	}
	question& test::get_question(size_t index) {
		if (index >= questions.size()) {
			throw test_question_not_found_exception("Questing at spicified index doesn't exist");
		}
		return questions[index];
	}
	const question& test::get_question(size_t index) const {
		return const_cast<test*>(this)->get_question(index);
	}
	void test::remove_question(size_t index) noexcept {
		questions.erase(questions.begin() + index);
	}
	void test::answer(size_t variant) noexcept {
		if (questions[i].corrent_variant == variant) {
			score++;
		}
		i++;
	}
	void test::reset() noexcept {
		i = 0;
		score = 0;
	}
	size_t test::get_score() const noexcept {
		return static_cast<size_t>((static_cast<double>(score) / static_cast<double>(questions.size())) * 12);
	}
	double test::get_pscore() const noexcept {
		return (static_cast<double>(score) / static_cast<double>(questions.size())) * 100.;
	}
	size_t test::get_rawscore() const noexcept {
		return score;
	}
	size_t test::get_high_rawscore() const noexcept {
		return questions.size();
	}
	size_t test::question_size() const noexcept {
		return questions.size();
	}
	test::operator bool() const noexcept {
		return i < questions.size();
	}
	serialize_data test::serialize() const {
		std::string data = "$";
		std::vector<serialize_data> sinfo;
		size_t questions_size = 0;
		{
			//metadata
			data.append(":" + std::to_string(category.size()));
			data.append(":" + std::to_string(name.size()));
			for (auto& q : questions) {
				size_t __question_size = 0;
				auto info = q.serialize();
				__question_size += info.size();
				sinfo.emplace_back(std::move(info));
				data.append(":" + std::to_string(__question_size));
			}
			data.push_back(';');
		}
		{
			//rawdata
			data.append(category);
			data.append(name);
			for (auto& sq : sinfo) {
				data.append(sq.get(), sq.size());
			}
		}
		auto data_size = data.size();
		{
			auto meta_info_size = (":" + std::to_string(data.size())).size();
			data_size += meta_info_size;
		}
		serialize_data info(data_size);
		data.insert(1, ":" + std::to_string(data_size));
		std::memmove(info.get(), data.c_str(), data_size);
		return info;
	}
	void test::deserialize(serialize_data& info) {
		std::string data;
		std::vector<std::string> metadata;
		{
			std::string rawdata;
			rawdata.assign(info.get(), info.size());
			auto split = stdstring_ext::split_first(rawdata, ";");
			if (split.size() != 2) {
				throw deserialize_exception("data cannot be deserialized due to incorrect syntax");
			}
			metadata = stdstring_ext::split(split[0], ":");
			data = split[1];
		}
		try {
			size_t main_shift_pos = 0;
			auto __category_size = static_cast<size_t>(std::stoi(metadata.at(2)));
			auto __name_size = static_cast<size_t>(std::stoi(metadata.at(3)));
			{
				category = data.substr(main_shift_pos, __category_size);
				main_shift_pos += __category_size;
				name = data.substr(main_shift_pos, __name_size);
				main_shift_pos += __name_size;
			}
			for (size_t i = 4; i < metadata.size(); i++) {
				question q;
				auto __question_size = static_cast<size_t>(std::stoi(metadata.at(i)));
				serialize_data info(__question_size);
				std::memmove(info.get(), data.substr(main_shift_pos, __question_size).c_str(), __question_size);
				q.deserialize(info);
				questions.push_back(q);
				main_shift_pos += __question_size;
			}
		} catch (...) {
			throw deserialize_exception("cannot deserialize data due to incorrect syntax of metadata");
		}
	}
	bool test::operator==(const test& rhs) const noexcept {
		return rhs.name == name && rhs.category == category;
	}
	bool test::operator!=(const test& rhs) const noexcept {
		return !operator==(rhs);
	}
	std::vector<question>::iterator test::begin() noexcept {
		return questions.begin();
	}
	std::vector<question>::iterator test::end() noexcept {
		return questions.end();
	}
	void test::set_rawscore(size_t score) noexcept {
		this->score = score;
	}
	size_t test::get_current_question_index() const noexcept {
		return i;
	}
	void test::set_current_question_index(size_t i) noexcept {
		this->i = i;
	}
}