#pragma once

#include <unordered_map>
#include "IWorldFactory.hpp"

class WorldFactoryManager {
public:
	void registerFactory(std::shared_ptr<IWorldFactory> factory);

	std::shared_ptr<IWorldFactory> findFactoryById(const std::string &id) const;

private:
	std::unordered_map<std::string, std::shared_ptr<IWorldFactory>> _factories;
};