#include "include/Weapon.hpp"

#include "include/Cooldown.hpp"

Weapon::Weapon(WeaponType type, const sf::Vector2f* pos, const sf::Vector2f* fac_vec, const TextureStorage* storage)
	: position(pos), facing_vec(fac_vec), txStorage(storage)
{
	sprite.setTexture(txStorage->GetTexture("Weapon"));
	angle = 0.f;

	//shape.setSize(sf::Vector2f(10 * sprite_scale, 4 * sprite_scale));
	//shape.setOrigin(shape.getSize().x / 2,
	//	shape.getSize().y / 2);
	//shape.setPosition(*position);
	//shape.setFillColor(sf::Color::Transparent);
	//shape.setOutlineColor(sf::Color::Cyan);
	//shape.setOutlineThickness(2);


	switch (type) {
	case WeaponType::wtSWORD: {
		sprite.setTextureRect(sf::IntRect(2, 2, 8, 20));
		sprite.setOrigin(4, 14);
		sprite.setScale(sprite_scale, sprite_scale);
		break;
	}
	case WeaponType::wtAXE: {
		sprite.setTextureRect(sf::IntRect(27, 2, 9, 20));
		sprite.setOrigin(3, 15);
		sprite.setScale(sprite_scale, sprite_scale);
		break;
	}
	default:
		std::cerr << "Unsupportable weapon type\n";
	}
}

void Weapon::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	//target.draw(shape, states);
}

void Weapon::update()
{
	if (facing_vec->x < 0) sprite.setScale(-sprite_scale, sprite_scale);
	else sprite.setScale(sprite_scale, sprite_scale);
	//shape.setPosition(*position + 10.f * turnNinety(*facing_vec, 1));
	sf::Vector2f new_pos = *position + 20.f * turnNinety(*facing_vec, 0);
	sprite.setPosition(new_pos.x, new_pos.y-10.f);
}
