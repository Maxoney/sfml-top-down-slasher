#include "include/Animation.hpp"

Animation::Animation(const sf::Sprite & sprite, float _switch_time, bool _horizontal, const float* delta_)
{
	Initialize(sprite, _switch_time, _horizontal, delta_);
}

void Animation::Initialize(const sf::Sprite & sprite, float _switch_time, bool _horizontal, const float* delta_)
{
	delta = delta_;
	frameBounds = sprite.getTextureRect();
	frameAmmount = { sprite.getTexture()->getSize().x / frameBounds.width,
					sprite.getTexture()->getSize().y / frameBounds.height };
	switch_time = _switch_time;
	currentFrame.x = 0;
	horizontal = _horizontal;
}

void Animation::update(int row)
{
	if (horizontal) {
		currentFrame.y = row;
		total_time += *delta;
		if (total_time >= switch_time) {
			total_time -= switch_time;
			currentFrame.x++;
			if (currentFrame.x >= frameAmmount.x) currentFrame.x = 0;
		}
	}
	else {
		currentFrame.x = row;	// column
		total_time += *delta;
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
