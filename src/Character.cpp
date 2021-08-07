#include "include/Character.hpp"

Character::Character(const sf::Texture& texture, float _x, float _y, const std::string _name, const GameDificulty dific, const ControlsType contr)
	: blade("res/sprites/blade.png", 3),max_hp(dific==EASY ? 20 : dific==NORMAL ? 10 : 1), max_sp(100)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 36, 26));
	sprite.setOrigin(36 / 2, 26 / 2);
	sprite.setPosition(0, 0); // settin' sprite spawn position
	sprite.setScale(2, 2);

	x = _x;
	y = _y;
	angle = 0.f;
	name = _name;
	hp = max_hp;
	sp = max_sp;
	cd_charge.SetTimer(600);
	cd_attack.SetTimer(300);
	cd_attack_anim.SetTimer(100);
	cd_powerup.SetTimer(7000);
	dmg = 2;
	if (contr == CONTROLS_FIXED) controls_fixed = true;
	else controls_fixed = false;
}

void Character::Controls()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { 
		dirup = 1; 
		state = WALKING_FORWARD; 
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { 
		dirdown = 1; 
		state = WALKING_BACK;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { 
		dirleft = 1; 
		state = WALKING_LSIDE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { 
		dirright = 1; 
		state = WALKING_RSIDE;
	}


//	charge = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 2 : 1;
	//				if (dirdown^dirleft^dirright^dirup) { sqrt2 = 1.41; }
	sqrt2 = dirdown^dirleft^dirright^dirup ? 1.41 : 1;
}

void Character::Charge(const float& delta, const sf::Event& event)
{
	///////// charge effect ///////// 700ms
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift && cd_charge.IsEnded() && !charging && sp >= 40) {
		cd_charge.StartCooldown();
		charging = true;
		sp -= 40;
		//if(state > 0 && state < 9 && state%2){ // если в состоянии ходьбы
		//	state = (CreatureState)(std::underlying_type<CreatureState>::type(state) + 1); // переход к следующему по порядку состоянию (бег)
		//}
	}
	/////////////////////////////////

}

void Character::CollisionDetection()
{
	if (x >= 1270) x = 1270;
	else if (x <= -1270) x = -1270; // 1280 -/+ 450			+ 5 пикселов для перестраховки (тряска и тд)
	if (y >= 1270) y = 1270;
	else if (y <= -1270) y = -1270; // 1280 -/+ 300
}

void Character::update(float& delta, sf::RenderWindow & window)
{
	sf::Vector2f worldPos = MouseWrldPos(window);	// шахер махер с переходом от стандартных оконных координат к обновленным
	facing_vec = Normalize2(worldPos - this->GetCords());	//	вектор смотрения персонажа
	angle = GetAngleOfNormVec2(facing_vec);
	sprite.setRotation(90 + angle * 57.32); //		180 / 3.14 = 57.32

	if (cd_powerup.IsEnded()) powerup = NONE;
	if (powerup == ENERGY_BOOST && sp < max_sp) sp++;

	if (cd_charge.IsEnded()){		 //charge < 1.f) {
		charge = 1.f;
		charging = false;
	} else  charge = 1 + 40 * EaseOutCubic(cd_charge.GetTime().asSeconds(), 0.5f);
	if (sp < max_sp + 1 && cd_charge.IsEnded() && cd_attack.IsEnded()) sp++;


	this->Controls();

	if (controls_fixed) {	// controlls are aligned to vector angle	(управление типа от первого лица играешь)
		dx = facing_vec.x*float(dirup) - facing_vec.y*float(dirright) + facing_vec.y*float(dirleft) - facing_vec.x*float(dirdown);
		dy = facing_vec.y*float(dirup) + facing_vec.x*float(dirright) - facing_vec.x*float(dirleft) - facing_vec.y*float(dirdown);
	}
	else {	// controlls are aligned to global coordinates system (нормальное управление)
		dx = float(dirright) - float(dirleft);
		dy = float(dirdown) - float(dirup);
	}
	x += dx * charge * sqrt2 * delta * speed;
	y += dy * charge * sqrt2 * delta * speed;
	this->CollisionDetection();
	sprite.setPosition(x, y);

	if(!cd_attack_anim.IsEnded()) blade.update(this->GetCords(), angle, delta);

	dirup = 0, dirdown = 0, dirleft = 0, dirright = 0;
//	std::cout << x << " \t " << y << "\n";
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!cd_attack_anim.IsEnded()) 	target.draw(blade, states);
	target.draw(sprite, states);
}

void Character::SetControls(const ControlsType & contr)
{
	if (contr == CONTROLS_FIXED) controls_fixed = true;
	else	controls_fixed = false;
}

void Character::SetDifficulty(const GameDificulty & diff)
{
	switch (diff) {
	case EASY: {
		max_hp = 20;
		hp = max_hp;
		break;
	}
	case NORMAL: {
		max_hp = 10;
		hp = max_hp;
		break;
	}
	case HARD: {
		max_hp = 1;
		hp = max_hp;
		break;
	}
	}
}

void Character::SetEnemiesAmount(const size_t & amount)
{
	enemies_amount = amount;
}

void Character::SetPosition(const float & _x, const float & _y)
{
	x = _x;
	y = _y;
}

void Character::PickUpPowerup(const PowerupType& type)
{
	powerup = type;
	cd_powerup.StartCooldown();
}

void Character::StaminaDrain(int & num)
{
	if (sp >= num) {
		sp -= num;
	}
	else sp = 0;
}

sf::Vector2f Character::GetCords() const
{
	return sf::Vector2f(x, y);
}

sf::Vector2f Character::GetFacingVec() const
{
	return facing_vec;
}

sf::Vector2i Character::GetHP() const
{
	return sf::Vector2i(max_hp, hp);
}

sf::Vector2i Character::GetSP() const
{
	return sf::Vector2i(max_sp, sp);
}

void Character::Attack()
{
	if (sp >= 35 && cd_attack.IsEnded()) {
		sp -= 35;
		cd_attack.StartCooldown();

		cd_attack_anim.StartCooldown();
		blade.GetAnimation()->restartAnimation();

		attacking = true;
	}
}

float Character::GetAngle() const
{
	return fmod(angle,6.28);
}

int Character::GetDmg() const
{
	return dmg;
}

int Character::GetEnemiesAmount() const
{
	return enemies_amount;
}

CreatureState Character::GetState() const
{
	return state;
}

Cooldown* Character::GetAttackCd()
{
	return (&cd_attack);
}

void Character::RecieveDmg(int& dmg)
{
	if (powerup != INVINCIBILITY) {
		hp -= dmg;
		if (hp < 1) {
			state = DEAD;
//			std::cout << "YOU ARE DEAD (OUTSIDE)\n";
		}
	}
}
