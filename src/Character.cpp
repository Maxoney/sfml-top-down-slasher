#include "include/Character.hpp"

#include "include/Weapon.hpp"

Character::Character(const TextureStorage* texture, float x_, float y_, const sf::RenderWindow* window_, const float* delta_)
	: blade("res/sprites/blade.png", 3, delta_), window(window_), delta(delta_), txStorage(texture)
{
	sprite.setTexture(txStorage->GetTexture("Character"));
	sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	sprite.setOrigin((16 >> 1), 14);
	sprite.setScale(sprite_scale, sprite_scale);
	position = { x_ , y_ };
	sprite.setPosition(position); // settin' sprite spawn position
	
	coll_rect.setSize(sf::Vector2f(10 * sprite_scale, 4 * sprite_scale));
	coll_rect.setOrigin(coll_rect.getSize().x / 2,
						coll_rect.getSize().y / 2);
	coll_rect.setPosition(position);
	coll_rect.setFillColor(sf::Color::Transparent);
	coll_rect.setOutlineColor(sf::Color::Cyan);
	coll_rect.setOutlineThickness(2);

	angle = 0.f;

	//	init hp & sp
	updateDifficulty();
	sp = { 100,100 };

	cd_charge.SetTimer(600);
	cd_attack.SetTimer(300);
	cd_attack_anim.SetTimer(100);
	cd_powerup.SetTimer(7000);
	dmg = 2;

	sword = new Weapon(WeaponType::wtSWORD, &position, &facing_vec, txStorage);
	animation.Initialize(sprite, 0.2, 1, delta);
	updateControls();
}

Character::~Character()
{
	delete sword;
}

void Character::Controls(const sf::Event* event = nullptr)
{
	if (event) {
		if (event->type == sf::Event::MouseButtonPressed && event->key.code == sf::Mouse::Left &&
			cd_attack.IsEnded() && sp.y >= 35) {
			Attack();
		}

		if (event->type == sf::Event::KeyPressed && event->key.code == sf::Keyboard::LShift && 
			cd_charge.IsEnded() && sp.y >= 40) {
			Charge();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { 
		dirup = 1; 
		state = CreatureState::WALKING;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { 
		dirdown = 1; 
		state = CreatureState::WALKING;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { 
		dirleft = 1; 
		state = CreatureState::WALKING;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { 
		dirright = 1; 
		state = CreatureState::WALKING;
	}


	//	charge = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 2 : 1;
	//				if (dirdown^dirleft^dirright^dirup) { sqrt2 = 1.41; }
	sqrt2 = dirdown^dirleft^dirright^dirup ? 1.41 : 1;



}

void Character::Movement()
{	
	state = CreatureState::IDLE;
	Controls();

	//	нахождение угла направления персонажа
	sf::Vector2f worldPos = MouseWrldPos(*window);	// шахер махер с переходом от стандартных оконных координат к мировым
	facing_vec = Normalize2(worldPos - this->GetCords());	//	вектор смотрения персонажа
	angle = GetAngleOfNormVec2(facing_vec);
	//sprite.setRotation(90 + angle * 57.32); //		180 / 3.14 = 57.32

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

	coll_rect.setPosition(position);

	CollisionDetection();
	//this->CollisionDetection();
	//CollisionDetection();
}

void Character::Charge()
{
	///////// charge effect ///////// 700ms
	cd_charge.StartCooldown();
	charging = true;
	sp.y -= 40;
	//if(state > 0 && state < 9 && state%2){ // если в состоянии ходьбы
	//	state = (CreatureState)(std::underlying_type<CreatureState>::type(state) + 1); // переход к следующему по порядку состоянию (бег)
	//}
	/////////////////////////////////

}

void Character::CollisionDetection()
{
	TerrainCollision(0, 0);
	TerrainCollision(1, 0);
	TerrainCollision(0, 1);
	TerrainCollision(1, 1);
}

void Character::update()
{

	if (cd_powerup.IsEnded()) powerup = NONE;
	if (powerup == ENERGY_BOOST && sp.y < sp.x) sp.y++;

	if (cd_charge.IsEnded()){		 //charge < 1.f) {
		charge = 1.f;
		charging = false;
	} else  charge = 1 + 20 * EaseOutCubic(cd_charge.GetTime().asSeconds(), 0.5f);
	if (sp.y < sp.x + 1 && cd_charge.IsEnded() && cd_attack.IsEnded()) sp.y++;


	this->Movement();

	animation.update(static_cast<int>(state));
	sprite.setTextureRect(animation.GetBounds());

	sword->update();
	if (facing_vec.x > 0) sprite.setScale(sprite_scale, sprite_scale);
	else sprite.setScale(-sprite_scale, sprite_scale);

	//sprite.setPosition(position);
	sprite.setPosition(std::floor(position.x),std::floor(position.y));

	if(!cd_attack_anim.IsEnded()) blade.update(this->GetCords(), angle, *delta);

	dirup = 0, dirdown = 0, dirleft = 0, dirright = 0;
	//std::cout << position.x << " \t " << position.y << "\n";
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!cd_attack_anim.IsEnded()) 	target.draw(blade, states);
	if (facing_vec.x > 0) {
		target.draw(sprite, states);
		if (cd_attack_anim.IsEnded())
			target.draw(*sword, states);
	}
	else {
		if (cd_attack_anim.IsEnded())
			target.draw(*sword, states);
		target.draw(sprite, states);
	}
	//target.draw(coll_rect, states);
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
		hp.x = 20;
		break;
	}
	case GameDificulty::NORMAL: {
		hp.x = 10;
		break;
	}
	case GameDificulty::HARD: {
		hp.x = 1;
		break;
	}
	}
}

void Character::SetPosition(const float & x_, const float & y_)
{
	position = { x_,y_ };
	coll_rect.setPosition(position);
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
	sp.y -= 35;
	cd_attack.StartCooldown();

	cd_attack_anim.StartCooldown();
	blade.GetAnimation()->restartAnimation();

	attacking = true;

}

float Character::GetAngle() const
{
	return fmod(angle,6.28);
}

int Character::GetDmg() const
{
	return dmg;
}

CreatureState Character::GetState() const
{
	return state;
}

void Character::TerrainCollision(bool left, bool up)
{
	sf::Vector2f hsize = coll_rect.getSize() * coll_rect.getScale().x / 2.f;

	if ((*collision_map)[CordsToIndex(coll_rect.getPosition().x + (1-2*left) * hsize.x,
		coll_rect.getPosition().y + (1-2*up) * hsize.y)]) {

		//std::cout << "COLLISION DETECTED!\n";


		sf::Vector2f corner = { coll_rect.getPosition().x + (1 - 2 * left) * hsize.x,
								coll_rect.getPosition().y + (1 - 2 * up) * hsize.y };
		sf::Vector2f tile_pos = { corner.x - static_cast<int>(corner.x) % GameSettings::tile_size + GameSettings::tile_size / 2,
									corner.y - static_cast<int>(corner.y) % GameSettings::tile_size + GameSettings::tile_size / 2 };
		sf::Vector2f cords_dif = coll_rect.getPosition() - tile_pos;

		//std::cout << up<< ' ' << left << std::endl
		//			<< tile_pos.x << ' ' << tile_pos.y << std::endl
		//			<< corner.x << ' ' << corner.y << std::endl;

		if (fabs(tile_pos.x - corner.x) < fabs(tile_pos.y - corner.y)) {
			position.y += (1 - 2 * !up) * (static_cast<float>(GameSettings::tile_size / 2) - fabs(tile_pos.y - corner.y));
			//position.y = std::floorf(position.y);
		}
		else {
			position.x += (1 - 2 * !left) * (static_cast<float>(GameSettings::tile_size / 2) - fabs(tile_pos.x - corner.x));
			//position.x = std::floorf(position.x);
		}

		//sprite.setPosition(position);
		coll_rect.setPosition(position);
	}

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

void Character::Reset()
{
	updateDifficulty();
	hp.y = hp.x;
	sp.y = sp.x;
}
