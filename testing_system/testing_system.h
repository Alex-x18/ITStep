#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <bitset>
#include <stdexcept>
#include <thread>
#include <chrono>
#define NS testing_system
#define USE_NS using namespace NS;
#define MAKE_EXCEPTION(exception_name) class exception_name : public std::exception { \
	public: \
		explicit exception_name(const std::string& message) noexcept: \
			msg(message) {} \
		virtual ~exception_name() noexcept {} \
		virtual const char* what() const noexcept override { \
			return msg.c_str(); \
		} \
	protected: \
		std::string msg; \
}
#define interface struct
#define abstract
namespace testing_system {
	namespace misc {
		using interrupt_throw_type = int;
		void interruptable_input(const std::string& msg, std::string& param);
		void input(const std::string& msg, std::string& param) noexcept;
		size_t filesize(const std::string& filename);
		extern const size_t default_sleep_duration;
		void clr_console();
		void sleep(size_t ms = default_sleep_duration);
	}
}