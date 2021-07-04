#include "include/Blade.hpp"


Blade::Blade(const std::string dir, float scale)
{
	texture.loadFromFile(dir);
	animation.Initialize(texture, { 1,4 }, 0.033, false);
	sprite.setTexture(texture);
	sprite.setTextureRect(animation.GetBounds());
	sprite.setOrigin(animation.GetBounds().width/2, animation.GetBounds().height);
	sprite.setScale({ scale,scale });
}

void Blade::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Blade::update(sf::Vector2f cords, float angle, float delta)
{
	sprite.setPosition(cords);
	sprite.setRotation(90 + angle * 57.32);
	animation.update(0, delta);
	sprite.setTextureRect(animation.GetBounds());
}

Animation * Blade::GetAnimation()
{
	return &animation;
}

