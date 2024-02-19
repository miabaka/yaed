#pragma once

#include <string_view>

class StringUtil {
public:
	static bool isDigit(std::string_view str);

	static bool isDigitOrEmpty(std::string_view str);
};