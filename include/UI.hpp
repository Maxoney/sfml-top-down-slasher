#pragma once
#include "SFML/Graphics.hpp"
#include "include/quick_functions.hpp"
#include "include/Character.hpp"
#include "include/Camera.hpp"


class UIComponent : public sf::Drawable {	// плохой класс, который я не хотел разделять на родительский и дочерние
public:
	UIComponent(const sf::Texture& texture, const Camera* camera_, int sx_, int sy_, int height_, int width_, float scale_);

	void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void update() = 0;

protected:
	const Camera* camera;
	sf::Font uifont;
	sf::Text text;
	sf::RectangleShape rect;
	sf::Sprite sprite;
	float x, y, scale;
	int sx, sy, height, width;
	bool first_time = true;

};

class UIHP : public UIComponent {
public:
	UIHP(const sf::Texture& texture, const Camera* camera, const sf::Vector2i* hp,
		int _sx, int _sy, int _height, int _width, float _scale);
	virtual void update() override;
private:
	const sf::Vector2i* hero_hp;
};

class UISP : public UIComponent {
public:
	UISP(const sf::Texture& texture, const Camera* camera, const sf::Vector2i* sp,
		int _sx, int _sy, int _height, int _width, float _scale);
	virtual void update() override;
	void setSP(const sf::Vector2i* sp) { hero_sp = sp; }
private:
	const sf::Vector2i* hero_sp;
};

class UIEnemyCounter : public UIComponent {
public:
	UIEnemyCounter(const sf::Texture& texture, const Camera* camera, const size_t* enemies,
		int _sx, int _sy, int _height, int _width, float _scale);
	virtual void update() override;
private:
	const size_t* enemy_count;
};

class UIIconAttack : public UIComponent {
public:
	UIIconAttack(const sf::Texture& texture, const Camera* camera, const Cooldown* cooldown,
		int _sx, int _sy, int _height, int _width, float _scale);
	virtual void update() override;
private:
	const Cooldown* cd_attack;
};

class UIIconCharge : public UIComponent {
public:
	UIIconCharge(const sf::Texture& texture, const Camera* camera, const Cooldown* cooldown,
		int _sx, int _sy, int _height, int _width, float _scale);
	virtual void update() override;

private:
	const Cooldown* cd_charge;
};

class UIPowerUp : public UIComponent {
public:
	UIPowerUp(const sf::Texture& texture, const Camera* camera, const Cooldown* cooldown, const PowerupType* type,
		int _sx, int _sy, int _height, int _width, float _scale);
	virtual void update() override;

private:
	const Cooldown* cd_powerup;
	const PowerupType* pu_type;
};
