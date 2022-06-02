#pragma once

#include "IGame.hpp"

class KnownGame : public IGame {
public:
	bool known() const override;

	const std::string &id() const override;

	const std::string &name() const override;

protected:
	KnownGame(std::string name, std::string id);

private:
	std::string _id;
	std::string _name;
};