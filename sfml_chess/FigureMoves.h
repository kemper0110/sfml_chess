#pragma once
#include <SFML/System/Vector2.hpp>
#include <variant>
#include "Figure.h"




namespace FigureMoves
{
	bool isDiagonalFree(const class Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	bool isVerticalFree(const class Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	bool isHorizontalFree(const class Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	bool isUnderAttackOf(const class Board& board, const sf::Vector2i& target, const Figure::Color color);
	bool isPat(const class Board& board, const Figure::Color color);
	bool isMat(class Board& board, const Figure::Color color);
	sf::Vector2i findKingOnBoard(const class Board& board, const Figure::Color color);
};
