#pragma once

#include <memory>
#include "Brush.hpp"

class BrushSelection {
public:
	std::shared_ptr<Brush> primary() const;

	void setPrimary(std::weak_ptr<Brush> brush);

	std::shared_ptr<Brush> secondary() const;

	void setSecondary(std::weak_ptr<Brush> brush);

private:
	std::weak_ptr<Brush> _primary;
	std::weak_ptr<Brush> _secondary;
};