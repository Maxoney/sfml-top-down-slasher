#pragma once
#include <SFML/Graphics.hpp>
#include "include/Character.hpp"
#include <queue>
//#include <iostream>
#include <cmath>
#include "include/quick_functions.hpp"

class Camera {
public:
	// accepts spawn coordinates & camera's tangible area
	Camera(const sf::RenderWindow* window);
	Camera(const sf::Vector2f& cords, const sf::RenderWindow* window);

	void CollisionDetection();
	// window resize handling
	void Resize(const sf::FloatRect &rect);
	void SetPos(const sf::Vector2f&);
	void SetMenuPos();
	// shakes camera on x and y axis
	void SetShake();
	// shakes camera spinning in random directions
	void SetRadialShake();
	void update();

	const sf::View& GetCamera() const;
	const sf::Vector2f* GetPos() const { return &smooth; }

	sf::Vector2f Floor() const { 
		return { std::floor(smooth.x), std::floor(smooth.y) };
	}

private:

private:
	const sf::RenderWindow* window;
	const sf::Vector2f* cords;
	Cooldown cd_shaking, cd_radialShaking;
	sf::Vector2f smooth = {0, 0};	// camera position
	sf::View camera;
	std::queue<sf::Vector2f> delay;	// очередь положений камеры для создания плавного слежения камеры за персонажем

	float k_chase = 25,		// коэффициент (плавности) слежения камеры за курсором
		k_delay = 15,		// коэффициент (плавности) отставания камеры от персонажа
		angle;
};