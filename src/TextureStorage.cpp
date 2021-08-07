#include "include/TextureStorage.hpp"

TextureStorage::TextureStorage()
{
	Insert("Background", "res/sprites/background.png");

	////////////	Utility
	Insert("Missing", "res/sprites/missing.png");

	////////////	UI
	Insert("Icons", "res/sprites/icons.png");
	Insert("Bars", "res/sprites/bars.png");

	////////////	Characters
	Insert("Character", "res/sprites/character.png");
	Insert("Zombie", "res/sprites/zombie.png");
	Insert("Chimera", "res/sprites/chimera.png");
}

const sf::Texture TextureStorage::GetTexture(std::string name) const
{
	if (storage.find(name) == storage.end()) {
		std::cerr << "No texture called " << name;
		return storage.at("Missing");
	}
	else return storage.at(name);
}

void TextureStorage::Insert(std::string name, std::string path)
{
	storage[name].loadFromFile(path);
	storage[name].setSmooth(false);
}
