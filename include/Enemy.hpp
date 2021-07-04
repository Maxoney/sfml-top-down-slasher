#pragma once
#include "SFML/Graphics.hpp"
#include "include/Cooldown.hpp"
#include "include/quick_functions.hpp"
#include "include/Character.hpp"

// ������������ ����� �������
class Enemy : public sf::Drawable {
public:
	// �����������, ������� ���������� ������ � �������������� ������� (� ��������)
	Enemy();

	virtual void update(float& delta, Character& hero) = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void RecieveDamage(int dmg);
	float GetAngle() const;
	std::string GetName() const;

	sf::Vector2f GetCords() const;
	sf::Vector2f GetFacingVec() const;
	CreatureState* GetState();

protected:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f facing_vec;
	CreatureState state;
	Cooldown cd_attack;
//	Animation anim_attack;

	float x, y, angle, fov, dist,	// ����������, ���� ��������� � ���� ���������, ��������� ����� ������ � �������
		dx, dy, speed;			// �������� �� � � �, � ����� ����������� ��������
	std::string name;
	bool inFOV;			// ����� � ���� ���������
	bool attacking;
	int hp, dmg, dmgResist;
};