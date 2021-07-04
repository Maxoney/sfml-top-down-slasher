#pragma once
#include <SFML/Graphics.hpp>
#include "include/Cooldown.hpp"
//#include <iostream>
#include <stdint.h>
#include "include/Blade.hpp"
#include "include/quick_functions.hpp"
#include "include/globalVar.hpp"
#include "include/Powerup.hpp"

class Character : public sf::Drawable {
public:
	// конструктор, который определяет спрайт и местоположение объекта (а также имя)
	Character(const std::string dir, float _x, float _y, const std::string _name, const GameDificulty dific, const ControlsType contr);

	// attack method
	void Attack();
	// finding out moving direction
	void Controls();
	// starting charge
	void Charge(const float& delta, const sf::Event& event);
	void ChangeControlsType() { controls_fixed^=true; };
	// detecting some collision
	void CollisionDetection();
	void update(float& delta, sf::RenderWindow& window);
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void SetControls(const ControlsType& contr);
	void SetDifficulty(const GameDificulty& diff);
	void SetEnemiesAmount(const size_t& amount); //	уберу когда поумнею (нужно для UI)
	void SetPosition(const float& _x, const float& _y);
	// handling powerup pickup
	void PickUpPowerup(const PowerupType& type);
	// handling stamina loose from external factors (currently not used)
	void StaminaDrain(int& num);
	void RecieveDmg(int& dmg);
	float GetAngle() const;
	int GetDmg() const;
	int GetEnemiesAmount() const;	// line 21


	sf::Vector2f GetCords() const;
	sf::Vector2f GetFacingVec() const;
	sf::Vector2i GetHP() const;
	sf::Vector2i GetSP() const;
	CreatureState GetState() const;
	Cooldown* GetAttackCd();

public:
	bool attacking = false;
	bool charging = false;

	CreatureState state = IDLE;
	Cooldown cd_charge, cd_attack, cd_powerup,
						cd_attack_anim;
	PowerupType powerup;

private:
	bool controls_fixed = false; // выключатель управления "типа от первого лица"
	bool dirup = 0, // direction
		dirdown = 0,
		dirleft = 0,
		dirright = 0;
	float x, y, angle; // координаты и угол смотрения персонажа
	float dx, dy;
	float sqrt2 = 1, charge = 1, speed = 50;// допоміжні параметры для передвижения персонажа
	std::string name;
	/* const */ int max_hp, max_sp;
	int hp, sp, dmg; 
	int enemies_amount; // в будущем этого не будет

	Blade blade;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f facing_vec;
};