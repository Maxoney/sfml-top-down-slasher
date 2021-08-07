#include "include/Camera.hpp"

Camera::Camera(const sf::Vector2f & _size)
{
	camera.setSize(_size);
	camera.setCenter(CENTER_WINDOW_X,CENTER_WINDOW_Y);
}

Camera::Camera(const sf::Vector2f& _cords, const sf::Vector2f& _size)
{
	camera.setSize(_size);
	for (int i = 0; i < k_delay; i++) {
		delay.push(_cords);
	}
	smooth = _cords;	
	camera.setCenter(smooth);
	cd_shaking.SetTimer(50); // 0.05 sec
	cd_radialShaking.SetTimer(50);
}

sf::View Camera::GetCamera() const
{
	return camera;
}

sf::Vector2f Camera::GetPos() const
{
	return smooth;
}

void Camera::CollisionDetection()
{
	if (smooth.x >= 825) smooth.x = 825;
	else if (smooth.x <= -825) smooth.x = -825; // 1280 -/+ 450			+ 5 пикселов для перестраховки (тряска и тд)
	if (smooth.y >= 975) smooth.y = 975;
	else if (smooth.y <= -975) smooth.y = -975; // 1280 -/+ 300
}

void Camera::Resize(const sf::FloatRect & rect)
{
	camera.reset(rect);
}

void Camera::SetPos(const sf::Vector2f & pos)
{
	camera.setCenter(pos);
}

void Camera::SetMenuPos()
{
	camera.setCenter(CENTER_WINDOW_X, CENTER_WINDOW_Y);
	camera.setRotation(0);
}

void Camera::SetShake()
{
	cd_shaking.StartCooldown();
}

void Camera::SetRadialShake()
{
	cd_radialShaking.StartCooldown();
}

void Camera::update(sf::Vector2f cords, sf::RenderWindow& window) // cords - координаты преследуемого объекта
{
	float x, y, dx, dy;
	sf::Vector2f dcords; // координаты курсора мыши
	dcords = MouseWrldPos(window);

	/*/////////////////////////////	не следит за курсором (работает как надо) 		
	x = smooth.x - (delay.front().x/ k_delay) + (cords.x/ k_delay);
	y = smooth.y - (delay.front().y/ k_delay) + (cords.y/ k_delay);
	smooth = { x, y };
	*//////////////////////////////

	//////////////////////////////	следит за курсосром (вроде работает как надо (а если и не работает, то никто не заметит))
//	dx = (cords.x * 0.9f + dcords.x * 0.1f) / k_chase - (delay.front().x / k_chase);	//дорого, но прикольно
//	dy = (cords.y * 0.9f + dcords.y * 0.1f) / k_chase - (delay.front().y / k_chase);
	x = smooth.x + (sf::Mouse::getPosition(window).x - WINDOW_WIDTH / 2) / 64.f;	//dx;	//дешевле, но тоже прикольно
	y = smooth.y + (sf::Mouse::getPosition(window).y - WINDOW_HEIGHT / 2) / 64.f;	//dy;
	if (cords != smooth) {
		smooth = { ((k_chase - 1)*x/k_chase + cords.x/k_chase),((k_chase - 1)*y/k_chase + cords.y/k_chase)  };
	} else smooth = { x,y };
	//////////////////////////////

	this->CollisionDetection();
	
	if (!cd_shaking.IsEnded()) {
		smooth.x += rand() % 6 - 3;
		smooth.y += rand() % 6 - 3;
	}
	if (cd_radialShaking.IsEnded()) {
		camera.setRotation(0);
	} else camera.setRotation(float(std::rand()%20)/10);


	camera.setCenter(smooth);
	delay.pop();
	delay.push(cords);
}


