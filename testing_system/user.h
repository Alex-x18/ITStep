#pragma once
#include "testing_system.h"
#include "repos.h"
namespace testing_system {
	enum class user_access_level : unsigned char {
		DEFAULT,
		ADMIN
	};
	std::ostream& operator<<(std::ostream&, const user_access_level& ual);
	class user final : public serializable, public cryptable {
		public:
			user() = default;
			user(user_access_level access_level, const std::string& surname, const std::string& name, const std::string& fathers_name, const std::string& phone, const std::string& login, const std::string& password):
				access_level(access_level),
				surname(surname),
				name(name),
				fathers_name(fathers_name),
				phone(phone),
				login(login),
				password(password) {}
			user_access_level get_access_level() const noexcept;
			void set_access_level(user_access_level access_level);
			void set_surname(const std::string& surname);
			void set_name(const std::string& name);
			void set_fathers_name(const std::string& fathers_name);
			void set_phone(const std::string& phone);
			void set_login(const std::string& login);
			void set_password(const std::string& password);
			const std::string& get_surname() const noexcept;
			const std::string& get_name() const noexcept;
			const std::string& get_fathers_name() const noexcept;
			const std::string& get_phone() const noexcept;
			const std::string& get_login() const noexcept;
			const std::string& get_password() const noexcept;
			bool operator==(const user& rhs) const noexcept;
			serialize_data serialize() const override;
			void deserialize(serialize_data& info) override;
			void encrypt() noexcept override;
			void decrypt() noexcept override;
		private:
			user_access_level access_level = user_access_level::DEFAULT;
			std::string name;
			std::string surname;
			std::string fathers_name;
			std::string phone;
			std::string login;
			std::string password;
	};
}