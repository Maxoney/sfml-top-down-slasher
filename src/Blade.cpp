#include "include/Blade.hpp"


Blade::Blade(const std::string dir, float scale, const float* delta_)
{
	texture.loadFromFile(dir);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0,34,19));
	sprite.setOrigin(sprite.getTextureRect().width/2, sprite.getTextureRect().height);
	sprite.setScale({ scale,scale });
	animation.Initialize(sprite, 0.033, false, delta_);
}

void Blade::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Blade::update(sf::Vector2f cords, float angle, float delta)
{
	sprite.setPosition(cords);
	sprite.setRotation(90 + angle * 57.32);
	animation.update(0);
	sprite.setTextureRect(animation.GetBounds());
}

Animation * Blade::GetAnimation()
{
	return &animation;
}

