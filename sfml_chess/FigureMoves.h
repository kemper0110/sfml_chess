#pragma once
#include <SFML/Graphics.hpp>
#include <variant>
#include "Figure.h"




namespace FigureMoves
{
	bool isDiagonalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	bool isVerticalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	bool isHorizontalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	bool isUnderAttackOf(const Board& board, const sf::Vector2i& target, const Figure::Color color);
	bool isPat(const Board& board, const Figure::Color color);
	bool isMat(Board& board, const Figure::Color color);
	sf::Vector2i findKingOnBoard(const Board& board, const Figure::Color color);
};
