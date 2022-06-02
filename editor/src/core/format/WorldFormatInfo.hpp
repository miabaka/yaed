#pragma once

#include <string>

class WorldFormatInfo {
public:
	WorldFormatInfo(std::string name, std::string fileExtension);

	const std::string &getFileExtension() const;

	const std::string &getName() const;

private:
	std::string _fileExtension;
	std::string _name;
};