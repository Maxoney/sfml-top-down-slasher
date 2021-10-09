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
	Character(const sf::Texture& texture, float _x, float _y, const sf::RenderWindow* window_, const float* delta_);

	// attack method
	void Attack();
	// finding out moving direction
	void Controls();
	// starting charge
	void Charge(const sf::Event* event);
	void ChangeControlsType() { controls_fixed^=true; };
	// detecting some collision
	void CollisionDetection();
	void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void updateControls();
	void updateDifficulty();
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


	const sf::Vector2f& GetCords() const;
	sf::Vector2f GetFacingVec() const;
	const sf::Vector2i* GetHP() const;
	const sf::Vector2i* GetSP() const;
	CreatureState GetState() const;
	const Cooldown* GetAttackCd() const { return &cd_attack; }
	const Cooldown* GetChargeCd() const { return &cd_charge; }
	const Cooldown* GetPowerUpCd() const { return &cd_powerup; }
	const PowerupType* GetPowerupType() const { return &powerup; }

public:
	CreatureState state = IDLE;
	Cooldown cd_charge, cd_attack, cd_powerup,
						cd_attack_anim;
	PowerupType powerup;

	bool attacking = false;
	bool charging = false;

private:
	const sf::RenderWindow* window;
	const float* delta;
	Blade blade;
	sf::Sprite sprite;
	std::string name;
	sf::Vector2i hp, sp;	//	{max_Xp, current_Xp}
	sf::Vector2f facing_vec;
	sf::Vector2f position;
	float angle; // угол смотрения персонажа
	float dx, dy;
	float sqrt2 = 1, charge = 1, speed = 50;// допоміжні параметры для передвижения персонажа
	int dmg; 
	int enemies_amount; // в будущем этого не будет
	bool controls_fixed = false; // выключатель управления "типа от первого лица"
	bool dirup = 0, // direction
		dirdown = 0,
		dirleft = 0,
		dirright = 0;

};