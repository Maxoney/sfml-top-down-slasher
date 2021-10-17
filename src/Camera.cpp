#include "include/Camera.hpp"

Camera::Camera(const sf::RenderWindow* window_) 
	:window(window_)
{
	camera.setSize(static_cast<float>(Resolution::w),
		static_cast<float>(Resolution::h));
	camera.setCenter(Resolution::w, Resolution::h);
}

Camera::Camera(const sf::Vector2f& cords_, const sf::RenderWindow* window_)
	: cords(&cords_), window(window_)
{
	camera.setSize(static_cast<float>(Resolution::w),
		static_cast<float>(Resolution::h));
	for (int i = 0; i < k_delay; i++) {
		delay.push(*cords);
	}
	smooth = *cords;	
	camera.setCenter(Floor());
	cd_shaking.SetTimer(50); // 0.05 sec
	cd_radialShaking.SetTimer(50);
}

const sf::View& Camera::GetCamera() const
{
	return camera;
}

void Camera::CollisionDetection()
{
	float xborder = GameSettings::level_width * GameSettings::tile_size - (Resolution::w / 2),
		yborder = GameSettings::level_height * GameSettings::tile_size - (Resolution::h / 2);
	if (smooth.x >= xborder) smooth.x = xborder;
	else if (smooth.x <= Resolution::w / 2) smooth.x = Resolution::w / 2;
	if (smooth.y >= yborder) smooth.y = yborder;
	else if (smooth.y <= Resolution::h / 2) smooth.y = Resolution::h / 2;
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
	camera.setCenter(Resolution::w>>1, Resolution::h>>1);
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

void Camera::update() // cords - координаты преследуемого объекта
{
	float x, y, dx, dy;

	x = smooth.x + (sf::Mouse::getPosition(*window).x - WINDOW_WIDTH / 2) / 128.f;	//dx;	//дешевле, но тоже прикольно
	y = smooth.y + (sf::Mouse::getPosition(*window).y - WINDOW_HEIGHT / 2) / 128.f;	//dy;
	if (*cords != smooth) {
		smooth = { ((k_chase - 1)*x/k_chase + cords->x/k_chase),((k_chase - 1)*y/k_chase + cords->y/k_chase)  };
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

	camera.setCenter(Floor());
	delay.pop();
	delay.push(*cords);
}


