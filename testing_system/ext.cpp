#include "ext.h"
namespace stdvector_ext {
	std::string join(const std::vector<std::string>& values, const std::string& separator) {
		std::string result;
		size_t i = 0;
		for (auto& v : values) {
			result += v;
			if (i != values.size() - 1) {
				result += separator;
			}
			i++;
		}
		return result;
	}
}