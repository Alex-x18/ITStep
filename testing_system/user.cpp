#include "user.h"
#include "ext.h"
namespace testing_system {
	std::ostream& operator<<(std::ostream& os, const user_access_level& ual) {
		switch (ual) {
			case user_access_level::ADMIN: {
				os << "ADMIN";
				break;
			}
			case user_access_level::DEFAULT: {
				os << "DEFAULT";
			}
		}
		return os;
	}
	user_access_level user::get_access_level() const noexcept {
		return access_level;
	}
	void user::set_access_level(user_access_level access_level) {
		this->access_level = access_level;
	}
	const std::string& user::get_name() const noexcept {
		return name;
	}
	void user::set_name(const std::string& name) {
		this->name = name;
	}
	const std::string& user::get_surname() const noexcept {
		return surname;
	}
	void user::set_surname(const std::string& surname) {
		this->surname = surname;
	}
	const std::string& user::get_fathers_name() const noexcept {
		return fathers_name;
	}
	void user::set_fathers_name(const std::string& fathers_name) {
		this->fathers_name = fathers_name;
	}
	const std::string& user::get_phone() const noexcept {
		return phone;
	}
	void user::set_phone(const std::string& phone) {
		this->phone = phone;
	}
	const std::string& user::get_login() const noexcept {
		return login;
	}
	void user::set_login(const std::string& login) {
		this->login = login;
	}
	const std::string& user::get_password() const noexcept {
		return password;
	}
	void user::set_password(const std::string& password) {
		this->password = password;
	}
	serialize_data user::serialize() const {
		std::string data = "$";
		const size_t __access_level_size = sizeof(user_access_level) * 8;
		std::bitset<__access_level_size> __access_level_bits(static_cast<std::underlying_type<user_access_level>::type>(access_level));
		{
			//metadata
			data.append(":" + std::to_string(__access_level_size));
			data.append(":" + std::to_string(surname.size()));
			data.append(":" + std::to_string(name.size()));
			data.append(":" + std::to_string(fathers_name.size()));
			data.append(":" + std::to_string(phone.size()));
			data.append(":" + std::to_string(login.size()));
			data.append(":" + std::to_string(password.size()));
			data.push_back(';');
		}
		{
			//rawdata
			data.append(__access_level_bits.to_string());
			data.append(surname);
			data.append(name);
			data.append(fathers_name);
			data.append(phone);
			data.append(login);
			data.append(password);
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
	void user::deserialize(serialize_data& info) {
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
			auto __access_level_size = static_cast<size_t>(std::stoi(metadata.at(2)));
			auto __surname_size = static_cast<size_t>(std::stoi(metadata.at(3)));
			auto __name_size = static_cast<size_t>(std::stoi(metadata.at(4)));
			auto __fathers_name_size = static_cast<size_t>(std::stoi(metadata.at(5)));
			auto __phone_size = static_cast<size_t>(std::stoi(metadata.at(6)));
			auto __login_size = static_cast<size_t>(std::stoi(metadata.at(7)));
			auto __password_size = static_cast<size_t>(std::stoi(metadata.at(8)));
			{
				access_level = static_cast<user_access_level>(std::bitset<sizeof(user_access_level) * 8>(data.substr(main_shift_pos, __access_level_size)).to_ulong());
				main_shift_pos += __access_level_size;
				surname = data.substr(main_shift_pos, __surname_size);
				main_shift_pos += __surname_size;
				name = data.substr(main_shift_pos, __name_size);
				main_shift_pos += __name_size;
				fathers_name = data.substr(main_shift_pos, __fathers_name_size);
				main_shift_pos += __fathers_name_size;
				phone = data.substr(main_shift_pos, __phone_size);
				main_shift_pos += __phone_size;
				login = data.substr(main_shift_pos, __login_size);
				main_shift_pos += __login_size;
				password = data.substr(main_shift_pos, __password_size);
			}
		} catch (...) {
			throw deserialize_exception("cannot deserialize data due to incorrect syntax of metadata");
		}
	}
	void user::encrypt() noexcept {
		static auto encrypt_field = [](auto& field) -> void {
			if (field.size() == 0) {
				return;
			}
			for (auto it = field.begin(), end = field.end(), kit = end - 1; it != end; it++) {
				*it ^= *kit;
				kit = it;
			}
		};
		encrypt_field(surname);
		encrypt_field(name);
		encrypt_field(fathers_name);
		encrypt_field(phone);
		encrypt_field(password);
	}
	void user::decrypt() noexcept {
		static auto decrypt_field = [](auto& field) -> void {
			if (field.size() == 0) {
				return;
			}
			for (auto it = field.rbegin(), end = field.rend(), kit = it + 1;; kit++, it++) {
				if (it == end - 1) {
					*it ^= *(field.end() - 1);
					break;
				}
				*it ^= *kit;
			}
		};
		decrypt_field(surname);
		decrypt_field(name);
		decrypt_field(fathers_name);
		decrypt_field(phone);
		decrypt_field(password);
	}
	bool user::operator==(const user& rhs) const noexcept {
		return login == rhs.login && password == rhs.password;
	}
}