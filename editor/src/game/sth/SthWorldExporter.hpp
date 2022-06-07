#pragma once

#include "../../core/export/BaseWorldExporter.hpp"

class SthWorldExporter : public BaseWorldExporter {
	const std::string &name() const override;

	std::set<std::string> supportedGameIds() const override;

	void save(const World &world, const std::filesystem::path &path) const override;
};