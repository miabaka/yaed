#pragma once

#include <string>

class StringUtil {
public:
	static bool isDigit(const std::string &str);

	static bool isDigitOrEmpty(const std::string &str);

	static int parseInt(const std::string &str);
};