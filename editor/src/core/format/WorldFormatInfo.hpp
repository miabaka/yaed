#pragma once

#include <string>

class WorldFormatInfo {
public:
	WorldFormatInfo(std::string name, std::string fileExtension);

	const std::string &fileExtension() const;

	const std::string &name() const;

	bool supportsWorldNames() const;

private:
	std::string _fileExtension;
	std::string _name;
};