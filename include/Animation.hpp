#pragma once
#include "SFML/Graphics.hpp"

class Animation {
public:
	Animation() = default;
	Animation(const sf::Sprite& sprite, float _switch_time, bool _horizontal, const float* delta_);

	void Initialize(const sf::Sprite& sprite, float _switch_time, bool _horizontal, const float* delta_);
	void update(int row);
	void restartAnimation();

	sf::IntRect GetBounds();

private:
	const float* delta;
	float total_time = 0, 
		switch_time;
	sf::IntRect frameBounds;
	sf::Vector2u frameAmmount,
				currentFrame;
	bool horizontal = true;

};