#include "WorldFormatInfo.hpp"

WorldFormatInfo::WorldFormatInfo(std::string name, std::string fileExtension)
		: _name(std::move(name)),
		  _fileExtension(std::move(fileExtension)) {}

const std::string &WorldFormatInfo::fileExtension() const {
	return _fileExtension;
}

const std::string &WorldFormatInfo::name() const {
	return _name;
}
