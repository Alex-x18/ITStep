#pragma once
#include "testing_system.h"
namespace stdstring_ext {
	template<class Container = std::vector<std::string>> Container split(std::string str, const std::string& separator) {
		size_t pos = 0;
		std::string token;
		Container result;
		while ((pos = str.find(separator)) != std::string::npos) {
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + separator.length());
		}
		result.push_back(str);
		return result;
	}
	template<class Container = std::vector<std::string>> Container split_first(std::string str, const std::string& seperator) {
		size_t pos = str.find(seperator);
		Container result;
		if (pos != std::string::npos) {
			result.push_back(str.substr(0, pos));
			str.erase(0, pos + 1);
			result.push_back(str);
		} else {
			result.push_back(str);
		}
		return result;
	}
}
namespace stdvector_ext {
	std::string join(const std::vector<std::string>& values, const std::string& separator);
	template<class T> void erase(std::vector<T>& vect, size_t index) {
		auto it = vect.begin();
		vect.erase(it + index);
	}
}