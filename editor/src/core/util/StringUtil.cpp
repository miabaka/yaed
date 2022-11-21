#include "StringUtil.hpp"

#include <algorithm>

bool StringUtil::isDigit(const std::string &str) {
	return std::all_of(str.begin(), str.end(), std::isdigit);
}

bool StringUtil::isDigitOrEmpty(const std::string &str) {
	return std::find_if(str.begin(), str.end(), [](char c) { return !std::isdigit(c); }) == str.end();
}

int StringUtil::parseInt(const std::string &str) {
	return std::atoi(str.c_str());
}