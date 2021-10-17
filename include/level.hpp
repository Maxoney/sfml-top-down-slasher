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
	Level(TextureStorage* _txStorage, Character* player_, Camera* camera_, float* delta_);
	~Level();

	void Init();
	bool update();
	void updateEnemies();
	void updateBullets();
	void updatePowerups();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	void buildMap();
	void buildMapCollision();
	void spawnCharacters();
	const bool& isRunning() const { return running; }
	void stopRunning() { running = false; }

private:
	TextureStorage* txStorage;
	Character* player;
	Camera* camera;
	float* delta;
	vec_uptrEnemy monsters;
	vec_uptrBullet clip;
	vec_uptrPowerup loot;
	vec_uptrUIComponent UI;
	std::vector<bool> map_collision;
	TileMap map_terrain,
			map_tdecoration,
			map_vdecoration,
			gmap_collision;

	size_t* enemies_ammount;
	bool running = false;
};