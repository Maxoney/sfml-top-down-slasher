#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class TextureStorage {
public:

	TextureStorage() {
		Insert("Background", "res/sprites/background.png");

		////////////	Utility
		Insert("Missing", "res/sprites/missing.png");

		////////////	UI
		Insert("Icons", "res/sprites/icons.png");
		Insert("Bars", "res/sprites/bars.png");

		////////////	Characters
		Insert("Character", "res/sprites/fHero_.png");
		Insert("Zombie", "res/sprites/orc_.png");
		Insert("Chimera", "res/sprites/ghost_.png");
		Insert("Tileset", "res/sprites/tilemap.png");
	}

	const sf::Texture& GetTexture(std::string name) const {
		if (storage.find(name) == storage.end()) {
			std::cerr << "No texture called " << name << std::endl;
			return storage.at("Missing");
		}
		else return storage.at(name);
	}

	void Insert(std::string name, std::string path) {
		sf::Texture texture;
		texture.loadFromFile(path);
		texture.setSmooth(false);
		storage[name] = texture;
	}

private:

	std::map<std::string, sf::Texture> storage;
};
