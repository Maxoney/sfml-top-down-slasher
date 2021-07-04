#pragma once
#include "SFML/Graphics.hpp"

class Animation {
public:
	Animation() = default;
	Animation(const sf::Texture& texture, sf::Vector2u _frameAmmount, float _switch_time, bool _horizontal);

	void Initialize(const sf::Texture& texture, sf::Vector2u _frameAmmount, float _switch_time, bool _horizontal);
	void update(int row, const float& delta);
	void restartAnimation();

	sf::IntRect GetBounds();

private:
	sf::IntRect frameBounds;
	sf::Vector2u frameAmmount,
				currentFrame;

	bool horizontal = true;
	float total_time = 0, 
		switch_time;
};