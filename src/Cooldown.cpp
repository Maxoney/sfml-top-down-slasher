#include "include/Cooldown.hpp"

Cooldown::Cooldown()
{
	milliseconds = 5000;
}

Cooldown::Cooldown(sf::Int32 cd_mill)
{
	milliseconds = cd_mill;
}

void Cooldown::StartCooldown()
{
	clock.restart();
	exists = true;
}

void Cooldown::SetTimer(sf::Int32 cd_mill)
{
	milliseconds = cd_mill;
}

const sf::Time Cooldown::GetTime() const
{
	return clock.getElapsedTime();
}

const bool Cooldown::IsEnded() const
{
	if (clock.getElapsedTime().asMilliseconds() >= milliseconds || !exists) {
		return true;
	}
	return false;
}
