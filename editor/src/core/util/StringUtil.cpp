#include "StringUtil.hpp"

#include <algorithm>
#include <cctype>

bool StringUtil::isDigit(std::string_view str) {
	return std::ranges::all_of(str, std::isdigit);
}

bool StringUtil::isDigitOrEmpty(std::string_view str) {
	return str.empty() || isDigit(str);
}