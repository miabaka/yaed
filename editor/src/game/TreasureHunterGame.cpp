#include "TreasureHunterGame.hpp"

TreasureHunterGame::TreasureHunterGame()
		: KnownGame("sth", "Snowy: Treasure Hunter") {
	addLayerTemplate("main", std::make_shared<LayerTemplate>("Main"));
	addLayerTemplate("gems", std::make_shared<LayerTemplate>("Gems"));
}