#include "include/level.hpp"
#include "include/Chimera.hpp"
#include "include/Zombie.hpp"
#include <iostream>
#include <fstream>

Level::Level(TextureStorage * _txStorage, Character* player_, const Camera* camera, float* delta_)
	: txStorage(_txStorage), player(player_), delta(delta_)
{
	enemies_ammount = new size_t(0);
	///////	UI ///////
	UI.push_back(std::make_unique<UIHP>(txStorage->GetTexture("Bars"), camera, 
		player->GetHP(), 0, 0, 13, 18, 4.f));
	UI.push_back(std::make_unique<UISP>(txStorage->GetTexture("Bars"), camera, 
		player->GetSP(), 0, 18, 12, 15, 4.f));
	UI.push_back(std::make_unique<UIEnemyCounter>(txStorage->GetTexture("Bars"), camera, 
		enemies_ammount, 12, 22, 13, 11, 2.f));
	UI.push_back(std::make_unique<UIIconAttack>(txStorage->GetTexture("Icons"), camera, 
		player->GetAttackCd(), 0, 0, 16, 16, 2.f));
	UI.push_back(std::make_unique<UIIconCharge>(txStorage->GetTexture("Icons"), camera, 
		player->GetChargeCd(), 16, 0, 16, 16, 2.f));
	UI.push_back(std::make_unique<UIPowerUp>(txStorage->GetTexture("Icons"), camera, 
		player->GetPowerUpCd(), player->GetPowerupType(), 32, 0, 16, 16, 2.f));

}

Level::~Level()
{
	delete enemies_ammount;
}

void Level::Init()
{
	buildMap();

	clip.clear();
	loot.clear();
	monsters.clear();

	spawnCharacters();
}

bool Level::update()
{
	for (auto &enemy : monsters)
		enemy->update();
	player->update();
	*enemies_ammount = monsters.size();
	for (auto &uiPart : UI)
		uiPart->update();
	return false;
}

void Level::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(map_terrain, states);
	target.draw(map_tdecoration, states);

	for(auto &enemy : monsters)
		target.draw(*enemy, states);

	target.draw(*player, states);

	target.draw(map_vdecoration, states);

	for(auto &uiPart : UI)
		target.draw(*uiPart, states);
}

void Level::buildMap()
{
	//sBackground->setTexture(txStorage->GetTexture("Background"));
	map_terrain.load(txStorage, "terrain");
	std::cout << "terrain loaded" << std::endl;
	map_terrain.setScale(4.f, 4.f);

	GameSettings::level_width = map_terrain.getWidth() * map_terrain.getScale().x;
	GameSettings::level_height = map_terrain.getHeight() * map_terrain.getScale().y;

	map_tdecoration.load(txStorage, "tdecoration");
	std::cout << "tdeco loaded" << std::endl;
	map_tdecoration.setScale(4.f, 4.f);
	map_vdecoration.load(txStorage, "vdecoration");
	std::cout << "vdeco loaded" << std::endl;
	map_vdecoration.setScale(4.f, 4.f);

	buildMapCollision();
	std::cout << "collisions loaded" << std::endl;

}

void Level::buildMapCollision()
{
	std::ifstream map("res/levels/level" + std::to_string(GameSettings::level_current) +
		"/level_settings.txt");

	unsigned int width, height;
	map >> width >> height;
	map_collision.reserve(width*height);

	map.open("res/levels/level" + std::to_string(GameSettings::level_current) +
		"/level_collision.txt");
	if (!map.is_open()) {
		std::cerr << "can't find: res/levels/level" + std::to_string(GameSettings::level_current) +
			"/level_collision.txt" << std::endl;
	}

	std::string str;
	int value;
	

	for (unsigned int i = 0; i < height; ++i) {
		std::getline(map, str);
		std::istringstream istr(str);
		for (unsigned int j = 0; j < width; ++j) {
			istr >> value;
			if (value == -1) map_collision.push_back(1);
			else map_collision.push_back(0);
		}
	}
}

void Level::spawnCharacters()
{
	std::ifstream map("res/levels/level" + std::to_string(GameSettings::level_current) +
		"/level_settings.txt");

	unsigned int width, height, tsize;
	map >> width >> height >> tsize;
	
	map.close();
	map.open("res/levels/level" + std::to_string(GameSettings::level_current) +
		"/level_spawnPoints.txt");
	if (!map.is_open()) {
		std::cerr << "can't find: res/levels/level" + std::to_string(GameSettings::level_current) +
			"/level_spawnPoints.txt" << std::endl;
	}

	int ch_type;

	for (unsigned int i = 0; i < height; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
			map >> ch_type;

			switch (ch_type) {
			case 427:
				monsters.push_back(std::make_unique<Zombie>(txStorage->GetTexture("Zombie"), player, delta));
				monsters.back()->setCords({ j * tsize * map_terrain.getScale().x,
										i * tsize * map_terrain.getScale().y });
				break;
			case 428:
				monsters.push_back(std::make_unique<Chimera>(txStorage->GetTexture("Chimera"), player, delta));
				monsters.back()->setCords({ j * tsize * map_terrain.getScale().x,
										i * tsize * map_terrain.getScale().y });
				break;
			case 453:
				player->SetPosition(j * tsize * map_terrain.getScale().x,
										i * tsize * map_terrain.getScale().y);
			}
		}
	}
	//// spawning monsters;
	//for (size_t i = 0; i < ZOMBIE_COUNT; ++i) {
	//	monsters.push_back(std::make_unique<Zombie>(txStorage->GetTexture("Zombie"), "zombie"));
	//}
	//for (size_t i = 0; i < CHIMERA_COUNT; ++i) {
	//	monsters.push_back(std::make_unique<Chimera>(txStorage->GetTexture("Chimera"), "chimera"));
	//}

}


