#include "include/Animation.hpp"

Animation::Animation(const sf::Texture & texture, sf::Vector2u _frameAmmount, float _switch_time, bool _horizontal)
{
	frameAmmount = _frameAmmount;
	switch_time = _switch_time;
	currentFrame.x = 0;
	horizontal = _horizontal;

	frameBounds.width  = float(texture.getSize().x) / float(frameAmmount.x);
	frameBounds.height = float(texture.getSize().y) / float(frameAmmount.y);
}

void Animation::Initialize(const sf::Texture & texture, sf::Vector2u _frameAmmount, float _switch_time, bool _horizontal)
{
	frameAmmount = _frameAmmount;
	switch_time = _switch_time;
	currentFrame.x = 0;
	horizontal = _horizontal;

	frameBounds.width = texture.getSize().x / float(frameAmmount.x);
	frameBounds.height = texture.getSize().y / float(frameAmmount.y);
}

void Animation::update(int row, const float & delta)
{
	if (horizontal) {
		currentFrame.y = row;
		total_time += delta;
		if (total_time >= switch_time) {
			total_time -= switch_time;
			currentFrame.x++;
			if (currentFrame.x >= frameAmmount.x) currentFrame.x = 0;
		}
	}
	else {
		currentFrame.x = row;	// column
		total_time += delta;
		if (total_time >= switch_time) {
			total_time -= switch_time;
			currentFrame.y++;
			if (currentFrame.y >= frameAmmount.y) currentFrame.y = 0;
		}
	}

	frameBounds.left = currentFrame.x * frameBounds.width;
	frameBounds.top = currentFrame.y * frameBounds.height;
}

void Animation::restartAnimation()
{
	currentFrame = { 0,0 };
}

sf::IntRect Animation::GetBounds()
{
	return frameBounds;
}
