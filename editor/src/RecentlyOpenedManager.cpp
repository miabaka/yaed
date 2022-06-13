#include "RecentlyOpenedManager.hpp"

#include <algorithm>

namespace fs = std::filesystem;

void RecentlyOpenedManager::pokePath(const fs::path &path) {
	removePath(path);
	_paths.push_back(path);
}

void RecentlyOpenedManager::removePath(const std::filesystem::path &path) {
	const auto it = std::find(_paths.begin(), _paths.end(), path);

	if (it == _paths.end())
		return;

	_paths.erase(it);
}

void RecentlyOpenedManager::clear() {
	_paths.clear();
}

const std::vector<std::filesystem::path> &RecentlyOpenedManager::paths() const {
	return _paths;
}
