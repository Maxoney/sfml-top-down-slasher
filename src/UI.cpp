#include "..\include\UI.hpp"


UIComponent::UIComponent(const sf::Texture& texture, const Camera* camera_, int sx_, int sy_, int height_, int width_, float scale_)
	: camera(camera_), sx(sx_), sy(sy_), height(height_), width(width_), scale(scale_)
{
	//type = _type;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(sx, sy, height, width));
	sprite.setOrigin(0, 0);
	sprite.setScale(sf::Vector2f(scale, scale));
	uifont.loadFromFile(FONT_LOCATION_FFF_FORWARD);
	text.setFont(uifont);

}

void UIComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(rect, states);
	target.draw(text, states);
}


UIHP::UIHP(const sf::Texture & texture, const Camera * camera, const sf::Vector2i* hp,
	int _sx, int _sy, int _height, int _width, float _scale)
	: UIComponent(texture, camera, _sx, _sy, _height, _width, _scale), hero_hp(hp)
{
	rect.setFillColor(sf::Color(217, 87, 99, 255));
	text.setCharacterSize(16);
	text.setFillColor(sf::Color(172, 50, 50, 255));
}

void UIHP::update()
{
	sf::Vector2f newPosition = camera->Floor() + sf::Vector2f(-(Resolution::w>>1) + 10, (Resolution::h >> 1) - (10 + height)*scale);
	sprite.setPosition(newPosition);
	rect.setPosition(newPosition + sf::Vector2f(scale, 17 * scale));
	if (hero_hp->y > 0) {
		rect.setSize(sf::Vector2f(11.f * scale, (-14 + (14.f / hero_hp->x)*(hero_hp->x - hero_hp->y)) * scale));
	}
	else rect.setSize(sf::Vector2f(11.f * scale, 0));

	text.setPosition(newPosition + sf::Vector2f(3.5 * scale, 8 * scale));
	text.setString(std::to_string(hero_hp->y));
	if (hero_hp->y < 10) text.setPosition(newPosition + sf::Vector2f(5 * scale, 8 * scale));
}


UISP::UISP(const sf::Texture & texture, const Camera * camera, const sf::Vector2i* sp,
	int _sx, int _sy, int _height, int _width, float _scale)
	: UIComponent(texture, camera, _sx, _sy, _height, _width, _scale), hero_sp(sp)
{
	sprite.setRotation(-90);
	rect.setFillColor(sf::Color(251, 242, 54, 255));
	text.setCharacterSize(8);
	text.setFillColor(sf::Color(48, 98, 48, 255));
}

void UISP::update()
{
	sf::Vector2f newPosition = camera->Floor() + sf::Vector2f(-(Resolution::w >> 1) + 10 + 15 * scale, (Resolution::h >> 1) - 20);
	sprite.setPosition(newPosition);
	rect.setPosition(newPosition + sf::Vector2f(2 * scale, -3 * scale));
	if (hero_sp->y > 0) {
		rect.setSize(sf::Vector2f((10.f / hero_sp->x)*(hero_sp->y + 10) * scale, scale));
	}
	else rect.setSize(sf::Vector2f(0, scale));
	text.setPosition(newPosition + sf::Vector2f(3 * scale, -8 * scale));
	text.setString(std::to_string(64 + hero_sp->x - hero_sp->y) + "bpm");

}


UIEnemyCounter::UIEnemyCounter(const sf::Texture & texture, const Camera * camera, const size_t* enemies,
	int _sx, int _sy, int _height, int _width, float _scale)
	:UIComponent(texture, camera, _sx, _sy, _height, _width, _scale), enemy_count(enemies)
{
	text.setCharacterSize(12);
}

void UIEnemyCounter::update()
{
	sf::Vector2f newPosition = camera->Floor() + sf::Vector2f(-(Resolution::w >> 1) + 10, -(Resolution::h >> 1) + 10);
	sprite.setPosition(newPosition);
	text.setPosition(newPosition + sf::Vector2f(20 * scale, 6));
	text.setString(std::to_string(*enemy_count) + " monsters left");

}

UIIconAttack::UIIconAttack(const sf::Texture & texture, const Camera * camera, const Cooldown* cooldown,
	int _sx, int _sy, int _height, int _width, float _scale)
	:UIComponent(texture, camera, _sx, _sy, _height, _width, _scale), cd_attack(cooldown)
{
	rect.setFillColor(sf::Color(150, 150, 150, 200));
	rect.setSize(sf::Vector2f(0, 0));
}

void UIIconAttack::update()
{
	sf::Vector2f newPosition = camera->Floor() + sf::Vector2f(-(Resolution::w >> 1) + 10, (Resolution::h >> 1) - (55 + 2 * height)*scale);
	sprite.setPosition(newPosition);
	rect.setPosition(newPosition + sf::Vector2f(0, height * scale));
	if (!cd_attack->IsEnded()) {
		rect.setSize(sf::Vector2f(width*scale, (-15.f + 0.05 * cd_attack->GetTime().asMilliseconds())*scale));
	}

}

UIIconCharge::UIIconCharge(const sf::Texture & texture, const Camera * camera, const Cooldown* cooldown,
		int _sx, int _sy, int _height, int _width, float _scale)
	:UIComponent(texture, camera, _sx, _sy, _height, _width, _scale), cd_charge(cooldown)
{
	rect.setFillColor(sf::Color(150, 150, 150, 200));
	rect.setSize(sf::Vector2f(0, 0));
}

void UIIconCharge::update()
{
	sf::Vector2f newPosition = camera->Floor() + sf::Vector2f(-(Resolution::w >> 1) + 10, (Resolution::h >> 1) - (50 + height)*scale);
	sprite.setPosition(newPosition);
	rect.setPosition(newPosition + sf::Vector2f(0.f, height * scale));
	if (!cd_charge->IsEnded()) {
		rect.setSize(sf::Vector2f(width*scale, (-15.f + 0.025 * cd_charge->GetTime().asMilliseconds())*scale));
	}
}

UIPowerUp::UIPowerUp(const sf::Texture& texture, const Camera* camera, const Cooldown* cooldown,
		const PowerupType* type, int _sx, int _sy, int _height, int _width, float _scale)
	:UIComponent(texture, camera, _sx, _sy, _height, _width, _scale), cd_powerup(cooldown), pu_type(type)
{
	rect.setFillColor(sf::Color(150, 150, 150, 200));
	rect.setSize(sf::Vector2f(0, 0));
}

void UIPowerUp::update()
{
	sf::Vector2f newPosition = camera->Floor() + sf::Vector2f(-(Resolution::w >> 1) + 10, -(Resolution::h >> 1) + 70);
	sprite.setPosition(newPosition);
	rect.setPosition(newPosition + sf::Vector2f(0.f, height * scale));
	if (*pu_type == PowerupType::ENERGY_BOOST)	sprite.setTextureRect(sf::IntRect(sx, sy, height, width));
	else if (*pu_type == PowerupType::INVINCIBILITY)	sprite.setTextureRect(sf::IntRect(sx + 16, sy, height, width));

	if (!cd_powerup->IsEnded()) {
		rect.setSize(sf::Vector2f(width*scale, (-16.f + 0.0022 * cd_powerup->GetTime().asMilliseconds())*scale));
	}
	else {
		sprite.setPosition(sf::Vector2f(-1500, -1500));
		rect.setSize(sf::Vector2f(0, 0));
	}

}
