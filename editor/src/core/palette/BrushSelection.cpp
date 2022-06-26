#include "BrushSelection.hpp"

std::shared_ptr<Brush> BrushSelection::primary() const {
	return _primary.lock();
}

void BrushSelection::setPrimary(std::weak_ptr<Brush> brush) {
	_primary = std::move(brush);
}

std::shared_ptr<Brush> BrushSelection::secondary() const {
	return _secondary.lock();
}

void BrushSelection::setSecondary(std::weak_ptr<Brush> brush) {
	_secondary = std::move(brush);
}