#pragma once
#include <SFML/Graphics.hpp>

class Cooldown {
public:

	Cooldown(); // Creates 5 sec cooldown
	Cooldown(sf::Int32 cd_mill ); // time in milliseconds			(	1000 ms = 1s	)

	void StartCooldown();
	void SetTimer(sf::Int32 cd_mill);
	// is cooldown ended?
	const bool IsEnded() const;

	// time passed since the cooldown start
	const sf::Time GetTime() const;
private:
	sf::Clock clock;
	sf::Int32 milliseconds;

	bool exists= false; 
};