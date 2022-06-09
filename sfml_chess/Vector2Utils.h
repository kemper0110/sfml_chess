#pragma once
#include <SFML/System/Vector2.hpp>

template<typename T>
sf::Vector2<T> operator*(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

template<typename T>
sf::Vector2<T> dot(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename T>
sf::Vector2<T> abs(const sf::Vector2<T>& v) {
	return { std::abs(v.x), std::abs(v.y) };
}


template<typename T>
T length(const sf::Vector2<T>& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

