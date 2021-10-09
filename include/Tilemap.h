#pragma once
#include <SFML/Graphics.hpp>
#include "include/TextureStorage.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

	bool load(TextureStorage* txstorage, const std::string type)
	{
		// load the tileset texture
		m_tileset = &txstorage->GetTexture("Tileset");

		std::ifstream map;
		map.open("res/levels/level" + std::to_string(GameSettings::level_current) +
				"/level_settings.txt");
		if (!map.is_open()) {
			std::cerr << "level_settings is not opened!" << std::endl;
		}

		map >> width >> height >> tileSize.x >> tileSize.y;
		resolution = { width, height };

		// resize the vertex array to fit the level size
		m_vertices.clear();
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(width * height * 4);
		//std::string path = "levels/level" + std::to_string(GameSettings::current_level) + "/level_" + type + ".txt";
		map.close();
		map.open("res/levels/level" + std::to_string(GameSettings::level_current) + 
			"/level_" + type + ".txt");

		if (!map.is_open()) {
			std::cerr << "can't find: res/levels/level" + std::to_string(GameSettings::level_current) +
				"/level_" + type + ".txt" << std::endl;
		}

		//map.open("levels/level" + std::to_string(GameSettings::current_level) + "/level_terrain" + ".txt");
		std::string str;

		for (unsigned int i = 0; i < height; ++i) {
			std::getline(map, str);
			std::istringstream istr(str);
			for (unsigned int j = 0; j < width; ++j) {
				// get the current tile number
				int tileNumber;
				istr >> tileNumber;

				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i * height + j) * 4];	//	width->height

				// define its 4 corners
				quad[0].position = sf::Vector2f(j * tileSize.x, i * tileSize.y);
				quad[1].position = sf::Vector2f((j + 1) * tileSize.x, i * tileSize.y);
				quad[2].position = sf::Vector2f((j + 1) * tileSize.x, (i + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(j * tileSize.x, (i + 1) * tileSize.y);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}
		}

		return true;
	}

	sf::Vector2u getResolution() { return resolution; }
	sf::Vector2u getTileSize() { return tileSize; }
	unsigned int getWidth() { return width * tileSize.x; }
	unsigned int getHeight() { return height * tileSize.y; }

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

private:
	sf::VertexArray m_vertices;
	const sf::Texture* m_tileset;

	sf::Vector2u resolution;
	sf::Vector2u tileSize;

	unsigned int width, height;
};