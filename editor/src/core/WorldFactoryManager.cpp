#include "WorldFactoryManager.hpp"

void WorldFactoryManager::registerFactory(std::shared_ptr<IWorldFactory> factory) {
	auto &id = factory->id();
	bool inserted = _factories.insert({id, std::move(factory)}).second;

	if (!inserted)
		throw std::runtime_error("World factory with id '" + id + "' is already registered");
}

std::shared_ptr<IWorldFactory> WorldFactoryManager::findFactoryById(const std::string &id) const {
	auto it = _factories.find(id);

	if (it == _factories.end())
		return {};

	return it->second;
}