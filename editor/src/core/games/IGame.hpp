#pragma once

#include <string>

class IGame {
public:
	virtual ~IGame() = default;

	virtual bool known() const = 0;

	virtual const std::string &id() const = 0;

	virtual const std::string &name() const = 0;
};