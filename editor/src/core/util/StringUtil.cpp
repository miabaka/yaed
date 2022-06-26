#include "StringUtil.hpp"

#include <algorithm>

bool StringUtil::isDigitOrEmpty(const std::string &str) {
	return std::find_if(str.begin(), str.end(), [](char c) { return !std::isdigit(c); }) == str.end();
}