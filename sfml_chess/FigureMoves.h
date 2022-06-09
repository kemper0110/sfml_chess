#pragma once
#include <SFML/Graphics.hpp>
#include "Figures_fwd.h"
#include "Figure.h"

class FigureMoves
{
public:
	static bool isDiagonalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	static bool isVerticalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	static bool isHorizontalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	static bool isKingShah(const Board& board, const sf::Vector2i& king);

	static bool isUnderAttackOf(const Board& board, const sf::Vector2i& target, const Figure::Color color);
	static bool isPat(const Board& board, const Figure::Color color);


	static sf::Vector2i findKingOnBoard(const Board& board);
	static bool canKingBeSecured(Board& board, const sf::Vector2i& king);
	static bool canFigureSecureKing(Board& board, const sf::Vector2i& security);
};

