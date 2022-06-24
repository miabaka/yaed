#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "game/IGame.hpp"
#include "Level.hpp"

class IWorldFactory;

class IWorldExporter;

class World {
public:
	friend class WorldUtil;

	explicit World(std::shared_ptr<IGame> game, std::string name = {});

	std::string &name();

	const std::string &name() const;

	const std::filesystem::path &path() const;

	const std::string &filename() const;

	const std::string &nameOrFilename() const;

	void setPath(const std::filesystem::path &path);

	const std::vector<std::shared_ptr<Level>> &levels() const;

	std::shared_ptr<IGame> game();

	std::shared_ptr<const IGame> game() const;

	std::shared_ptr<IWorldFactory> factory() const;

	void setFactory(std::shared_ptr<IWorldFactory> factory);

	std::shared_ptr<IWorldExporter> exporter() const;

	void setExporter(std::shared_ptr<IWorldExporter> exporter);

	bool removeMarkedLevels();

private:
	std::string _name;
	std::string _filename;
	std::string _nameWithFilename;
	std::filesystem::path _path;
	std::shared_ptr<IWorldExporter> _exporter;
	std::shared_ptr<IWorldFactory> _factory;
	std::shared_ptr<IGame> _game;
	std::vector<std::shared_ptr<Level>> _levels;

	std::shared_ptr<Level> createLevel(std::shared_ptr<LevelSkin> skin = {}, std::string name = {});
};