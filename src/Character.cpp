#include "include/Character.hpp"

Character::Character(const sf::Texture& texture, float x_, float y_, const sf::RenderWindow* window_, const float* delta_)
	: blade("res/sprites/blade.png", 3), window(window_), delta(delta_)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 36, 26));
	sprite.setOrigin(36 / 2, 26 / 2);
	sprite.setPosition(0, 0); // settin' sprite spawn position
	sprite.setScale(2, 2);

	position = { x_,y_ };

	angle = 0.f;

	//	init hp & sp
	updateDifficulty();
	sp = { 100,100 };

	cd_charge.SetTimer(600);
	cd_attack.SetTimer(300);
	cd_attack_anim.SetTimer(100);
	cd_powerup.SetTimer(7000);
	dmg = 2;

	updateControls();
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

void Character::Charge(const sf::Event* event)
{
	///////// charge effect ///////// 700ms
	if (event->type == sf::Event::KeyPressed && event->key.code == sf::Keyboard::LShift && cd_charge.IsEnded() && !charging && sp.y >= 40) {
		cd_charge.StartCooldown();
		charging = true;
		sp.y -= 40;
		//if(state > 0 && state < 9 && state%2){ // если в состоянии ходьбы
		//	state = (CreatureState)(std::underlying_type<CreatureState>::type(state) + 1); // переход к следующему по порядку состоянию (бег)
		//}
	}
	/////////////////////////////////

}

void Character::CollisionDetection()
{
	if (position.x >= 1270) position.x = 1270;
	else if (position.x <= -1270) position.x = -1270; // 1280 -/+ 450			+ 5 пикселов для перестраховки (тряска и тд)
	if (position.y >= 1270) position.y = 1270;
	else if (position.y <= -1270) position.y = -1270; // 1280 -/+ 300
}

void Character::update()
{
	sf::Vector2f worldPos = MouseWrldPos(*window);	// шахер махер с переходом от стандартных оконных координат к обновленным
	facing_vec = Normalize2(worldPos - this->GetCords());	//	вектор смотрения персонажа
	angle = GetAngleOfNormVec2(facing_vec);
	sprite.setRotation(90 + angle * 57.32); //		180 / 3.14 = 57.32

	if (cd_powerup.IsEnded()) powerup = NONE;
	if (powerup == ENERGY_BOOST && sp.y < sp.x) sp.y++;

	if (cd_charge.IsEnded()){		 //charge < 1.f) {
		charge = 1.f;
		charging = false;
	} else  charge = 1 + 40 * EaseOutCubic(cd_charge.GetTime().asSeconds(), 0.5f);
	if (sp.y < sp.x + 1 && cd_charge.IsEnded() && cd_attack.IsEnded()) sp.y++;


	this->Controls();

	if (controls_fixed) {	// controlls are aligned to vector angle	(управление типа от первого лица играешь)
		dx = facing_vec.x*float(dirup) - facing_vec.y*float(dirright) + facing_vec.y*float(dirleft) - facing_vec.x*float(dirdown);
		dy = facing_vec.y*float(dirup) + facing_vec.x*float(dirright) - facing_vec.x*float(dirleft) - facing_vec.y*float(dirdown);
	}
	else {	// controlls are aligned to global coordinates system (нормальное управление)
		dx = float(dirright) - float(dirleft);
		dy = float(dirdown) - float(dirup);
	}
	position.x += dx * charge * sqrt2 * *delta * speed;
	position.y += dy * charge * sqrt2 * *delta * speed;
	//this->CollisionDetection();
	sprite.setPosition(position.x, position.y);

	if(!cd_attack_anim.IsEnded()) blade.update(this->GetCords(), angle, *delta);

	dirup = 0, dirdown = 0, dirleft = 0, dirright = 0;
	//std::cout << position.x << " \t " << position.y << "\n";
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!cd_attack_anim.IsEnded()) 	target.draw(blade, states);
	target.draw(sprite, states);
}

void Character::updateControls()
{
	if (GameSettings::controls_type == ControlsType::CONTROLS_FIXED) controls_fixed = true;
	else	controls_fixed = false;
}

void Character::updateDifficulty()
{
	switch (GameSettings::game_difficulty) {
	case GameDificulty::EASY: {
		hp = { 20,20 };
		break;
	}
	case GameDificulty::NORMAL: {
		hp = { 10,10 };
		break;
	}
	case GameDificulty::HARD: {
		hp = { 1,1 };
		break;
	}
	}
}

void Character::SetEnemiesAmount(const size_t & amount)
{
	enemies_amount = amount;
}

void Character::SetPosition(const float & x_, const float & y_)
{
	position = { x_,y_ };
}

void Character::PickUpPowerup(const PowerupType& type)
{
	powerup = type;
	cd_powerup.StartCooldown();
}

void Character::StaminaDrain(int & num)
{
	if (sp.y >= num) {
		sp.y -= num;
	}
	else sp.y = 0;
}

const sf::Vector2f& Character::GetCords() const
{
	return position;
}

sf::Vector2f Character::GetFacingVec() const
{
	return facing_vec;
}

const sf::Vector2i* Character::GetHP() const
{
	return &hp;
}

const sf::Vector2i* Character::GetSP() const
{
	return &sp;
}

void Character::Attack()
{
	if (sp.y >= 35 && cd_attack.IsEnded()) {
		sp.y -= 35;
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

void Character::RecieveDmg(int& dmg)
{
	if (powerup != INVINCIBILITY) {
		hp.y -= dmg;
		if (hp.y < 1) {
			state = DEAD;
		}
	}
}
