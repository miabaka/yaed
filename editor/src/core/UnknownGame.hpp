#pragma once

#include "IGame.hpp"

class UnknownGame : public IGame {
	explicit UnknownGame(std::string id);

	bool known() const override;

	const std::string &id() const override;

	const std::string &name() const override;

private:
	std::string _id;
};