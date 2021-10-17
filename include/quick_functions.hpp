#pragma once
#include <cmath>
#include <cstdlib>
//#include <iostream>
#include "include/globalVar.hpp"
#include <ctime>

static const float DotProduct (const sf::Vector2f& lhs, const sf::Vector2f& rhs) {  // dot product
	return lhs.x*rhs.x+lhs.y*rhs.y;
}

static const sf::Vector2f Normalize2(const sf::Vector2f& vec) {
	return { vec.x / sqrt(vec.x*vec.x + vec.y*vec.y), vec.y / sqrt(vec.x*vec.x + vec.y*vec.y) };
}

static float EaseOutCubic(const float& time, const float& duration) { // charge easing function (<1sec) (функция для определённого типа передвижения / анимации)
	return (duration - time)*(duration - time);// *(duration - time);
};

static const float EaseOutQuint(const float& time, const float& duration) {
	float x = duration - time;
	if (x < 0.5) {
		return 16 * x * x * x * x * x;
	}
	else {
		x = -2 * x + 2;
		return 1 - x * x * x * x * x / 2;
	}
//	return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}

static const float GetAngleBP(const sf::Vector2f& first, const sf::Vector2f& second) {
	return std::atan2(first.y - second.y, first.x - second.x);
}

static const float GetAngleOfNormVec2(const sf::Vector2f& vec) {
	if (vec.y < 0) return -acos(DotProduct(vec, sf::Vector2f(1, 0)));
	else return acos(DotProduct(vec, sf::Vector2f(1, 0)));
}

static const float GetAngleOfVec2(const sf::Vector2f& _vec) {
	return GetAngleOfNormVec2(Normalize2(_vec));
}

static const float GetAngleBV2(const sf::Vector2f& first, const sf::Vector2f& second) {
	return acos(DotProduct(first,second));
}

// Get Distance Between Points
static const float GetDistanceBP(const sf::Vector2f& first, const sf::Vector2f& second) {
	float a = second.x - first.x,
		  b = second.y - first.y;
	return sqrt(a * a + b * b);
}

static const float GetFastDistanceBP(const sf::Vector2f& first, const sf::Vector2f& second) {
	float a = second.x - first.x,
		b = second.y - first.y;
	return a * a + b * b;
}

static const sf::Vector2f MouseWrldPos(const sf::RenderWindow& window) {
	// get the current mouse position in the window
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

	// convert it to world coordinates
	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
	return worldPos;
}

static sf::Vector2f RandomCords() {
	return { float(std::rand() % 2400 - 1200),float(std::rand() % 2400 - 1200) };
}

template<typename T>
static size_t CordsToIndex(T x, T y) {
	return (static_cast<unsigned int>(x) / GameSettings::tile_size +
		(static_cast<unsigned int>(y) / GameSettings::tile_size) * GameSettings::level_width);
}