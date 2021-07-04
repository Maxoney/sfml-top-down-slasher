#include "..\include\Powerup.hpp"

Powerup::Powerup(const sf::Vector2f& cords)
{
	//sf::Texture texture;
	texture.loadFromFile("res/sprites/powerups.png");
	texture.setSmooth(false);
	sprite.setTexture(texture);
	lifetime.SetTimer(5000);
	x = cords.x;
	y = cords.y;
	if (rand() % 2) {
		type = INVINCIBILITY;
		sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
	}
	else {
		type = ENERGY_BOOST;
		sprite.setTextureRect(sf::IntRect(8, 0, 8, 8));
	}
	sprite.setOrigin(4, 4);
	sprite.setScale({ 3,3 });
	sprite.setPosition(x, y);
	lifetime.StartCooldown();
}

void Powerup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

sf::Vector2f Powerup::GetCords()
{
	return sf::Vector2f(x, y);
}

PowerupType Powerup::GetType()
{
	return type;
}

bool Powerup::IsDead()
{
	return lifetime.IsEnded();
}
