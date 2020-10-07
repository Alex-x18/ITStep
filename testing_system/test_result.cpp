#include "test_result.h"
#include "ext.h"
namespace testing_system {
	bool test_result::operator==(const test_result& rhs) const noexcept {
		return rhs.test_category == test_category && rhs.test_name == test_name && rhs.user_score == user_score && rhs.user_login == user_login;
	}
	serialize_data test_result::serialize() const {
		std::string data;
		auto __test_score = std::to_string(user_score);
		{
			//metadata
			data.append(":" + test_category.size());
			data.append(":" + test_name.size());
			data.append(":" + __test_score.size());
			data.append(":" + user_login.size());
			data.push_back(';');
		}
		{
			//rawdata
			data.append(test_category);
			data.append(test_name);
			data.append(__test_score);
			data.append(user_login);
		}
		auto data_size = data.size();
		{
			auto meta_info_size = (":" + std::to_string(data_size)).size();
			data_size += meta_info_size;
		}
		serialize_data info(data_size);
		std::memmove(info.get(), data.c_str(), data_size);
		return info;
	}
	void test_result::deserialize(serialize_data& info) {
		std::string data;
		std::vector<std::string> metadata;
		{
			std::string rawdata;
			rawdata.assign(info.get(), info.size());
			auto split = stdstring_ext::split_first(rawdata, ";");
			if (split.size() != 2) {
				throw deserialize_exception("given data cannot be deserialize due to parsing error");
			}
			metadata = stdstring_ext::split(split[0], ":");
			data = split[1];
		}
		try {
			size_t main_shift_pos = 0;
			auto __test_category_size = static_cast<size_t>(std::stoi(metadata.at(2)));
			auto __test_name_size = static_cast<size_t>(std::stoi(metadata.at(3)));
			auto __user_score_size = static_cast<size_t>(std::stoi(metadata.at(4)));
			auto __user_login_size = static_cast<size_t>(std::stoi(metadata.at(5)));
			{
				test_category = data.substr(main_shift_pos, __test_category_size);
				main_shift_pos += __test_category_size;
				test_name = data.substr(main_shift_pos, __test_name_size);
				main_shift_pos += __test_name_size;
				user_score = static_cast<size_t>(std::stoi(data.substr(main_shift_pos, __user_score_size)));
				main_shift_pos += __user_score_size;
				user_login = data.substr(main_shift_pos, __user_login_size);
			}
		} catch (...) {
			throw deserialize_exception("diven metadata contains error");
		}
	}
}