#include "..\include\UI.hpp"


UIComponent::UIComponent(const std::string dir, UIComponentType _type, int _sx, int _sy, int _height, int _width, float _scale)
{
	type = _type;
	sx = _sx;
	sy = _sy;
	scale = _scale;
	height = _height;
	width = _width;
	texture.loadFromFile(dir);
	texture.setSmooth(false);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(sx, sy, height, width));
	sprite.setOrigin(0, 0);
	sprite.setScale(sf::Vector2f(scale, scale));
	uifont.loadFromFile(FONT_LOCATION_FFF_FORWARD);
	text.setFont(uifont);

	switch (type) {
	case BAR_HP: {
		rect.setFillColor(sf::Color(217, 87, 99, 255));
		text.setCharacterSize(16);
		text.setFillColor(sf::Color(172, 50, 50, 255));
		break; 
	}
	case BAR_SP: {
		sprite.setRotation(-90);
		rect.setFillColor(sf::Color(251, 242, 54, 255)); 
		text.setCharacterSize(8);
		text.setFillColor(sf::Color(48, 98, 48, 255));
		break;
	}
	case BAR_ENEMY_COUNTER: {
		text.setCharacterSize(12);
		break;
	}
	default: {
		rect.setFillColor(sf::Color(150, 150, 150, 200));
		rect.setSize(sf::Vector2f(0, 0));
		break; 
	}
	}
}

void UIComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(rect, states);
	target.draw(text, states);
}

void UIComponent::update(const sf::Vector2f& cords, const Character& hero)
{
	switch (type) {
	case BAR_HP: {
		sf::Vector2f newPosition = cords + sf::Vector2f(-CENTER_WINDOW_X + 10, CENTER_WINDOW_Y - (10 + height)*scale);
		sprite.setPosition(newPosition);
		rect.setPosition(newPosition + sf::Vector2f(scale, 17 * scale));
		if (hero.GetHP().y > 0) {
			rect.setSize(sf::Vector2f(11.f * scale, (-14 + (14.f / hero.GetHP().x)*(hero.GetHP().x - hero.GetHP().y)) * scale));
		} else rect.setSize(sf::Vector2f(11.f * scale, 0));

		text.setPosition(newPosition + sf::Vector2f(3.5 * scale, 8 * scale));
		text.setString(std::to_string(hero.GetHP().y));
		if(hero.GetHP().y<10) text.setPosition(newPosition + sf::Vector2f(5 * scale, 8 * scale));
		break;
	}
	case BAR_SP: {
		sf::Vector2f newPosition = cords + sf::Vector2f(-CENTER_WINDOW_X + 10 + 15*scale, CENTER_WINDOW_Y - 20);
		sprite.setPosition(newPosition);
		rect.setPosition(newPosition + sf::Vector2f(2*scale, -3 * scale));
		if (hero.GetSP().y > 0) {
			rect.setSize(sf::Vector2f((10.f/hero.GetSP().x)*(hero.GetSP().y+10) * scale, scale ));
		} else rect.setSize(sf::Vector2f(0, scale));
		text.setPosition(newPosition + sf::Vector2f(3 * scale, -8 * scale));
		text.setString(std::to_string(64+hero.GetSP().x- hero.GetSP().y)+"bpm");

		break;
	}
	case BAR_ENEMY_COUNTER: {
		sf::Vector2f newPosition = cords + sf::Vector2f(-CENTER_WINDOW_X + 10, -CENTER_WINDOW_Y + 10);
		sprite.setPosition(newPosition);
		text.setPosition(newPosition + sf::Vector2f(20 * scale, 6));
		text.setString(std::to_string(hero.GetEnemiesAmount()) + " monsters left");
		break;
	}
	case ICON_ATTACK: {
		sf::Vector2f newPosition = cords + sf::Vector2f(-CENTER_WINDOW_X + 10, CENTER_WINDOW_Y - (55 + 2 * height)*scale);
		sprite.setPosition(newPosition);
		rect.setPosition(newPosition + sf::Vector2f( 0, height * scale));
		if (!hero.cd_attack.IsEnded()) {
			rect.setSize(sf::Vector2f(width*scale, (-15.f + 0.05*hero.cd_attack.GetTime().asMilliseconds())*scale));	
		}
		break;
	}
	case ICON_CHARGE: {
		sf::Vector2f newPosition = cords + sf::Vector2f(-CENTER_WINDOW_X + 10, CENTER_WINDOW_Y - (50 + height)*scale);
		sprite.setPosition(newPosition);
		rect.setPosition(newPosition + sf::Vector2f(0, height * scale));
		if (!hero.cd_charge.IsEnded()) {
			rect.setSize(sf::Vector2f(width*scale, (-15.f + 0.025*hero.cd_charge.GetTime().asMilliseconds())*scale));
		}
		break;
	}
	case ICON_POWERUP: {
		sf::Vector2f newPosition = cords + sf::Vector2f(-CENTER_WINDOW_X + 10, -CENTER_WINDOW_Y + 70);
		sprite.setPosition(newPosition);
		rect.setPosition(newPosition + sf::Vector2f(0, height * scale));
		if (hero.powerup==ENERGY_BOOST)	sprite.setTextureRect(sf::IntRect(sx, sy, height, width));
		else if (hero.powerup == INVINCIBILITY)	sprite.setTextureRect(sf::IntRect(sx + 16, sy, height, width));

		if (!hero.cd_powerup.IsEnded()) {
			rect.setSize(sf::Vector2f(width*scale, (-16.f + 0.0022*hero.cd_powerup.GetTime().asMilliseconds())*scale));
		}
		else {
			sprite.setPosition(sf::Vector2f(1500, 1500));
			rect.setSize(sf::Vector2f(0, 0));
		}
		break;
	}
	}
}