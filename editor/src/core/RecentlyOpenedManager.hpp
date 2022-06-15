#pragma once

#include <filesystem>
#include <string>
#include <vector>

class RecentlyOpenedManager {
public:
	void pokePath(const std::filesystem::path &path);

	void removePath(const std::filesystem::path &path);

	void clear();

	const std::vector<std::filesystem::path> &paths() const;

private:
	std::vector<std::filesystem::path> _paths;
};