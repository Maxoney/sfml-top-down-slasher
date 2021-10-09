#pragma once
#include "include/globalVar.hpp"
#include "include/quick_functions.hpp"
#include "include/Character.hpp"
#include "include/Enemy.hpp"
#include "include/Bullet.hpp"
#include "include/Tilemap.h"
#include "include/UI.hpp"

class Level : public sf::Drawable {
public:
	Level(TextureStorage* _txStorage, Character* player_, const Camera* camera, float* delta_);
	~Level();

	void Init();
	bool update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	void buildMap();
	void buildMapCollision();
	void spawnCharacters();

private:
	TextureStorage* txStorage;
	Character* player;
	float* delta;
	std::vector<std::unique_ptr<Enemy>> monsters;
	std::vector<std::unique_ptr<Bullet>> clip;
	std::vector<std::unique_ptr<Powerup>> loot;
	std::vector<std::unique_ptr<UIComponent>> UI;
	std::vector<bool> map_collision;
	TileMap map_terrain,
			map_tdecoration,
			map_vdecoration;

	size_t* enemies_ammount;
};