#pragma once
#include <SFML/Graphics.hpp>
#include <variant>

#include "Figures_fwd.h"
#include "Figure.h"

class FigureMoves
{
public:
	static bool isDiagonalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	static bool isVerticalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);
	static bool isHorizontalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst);

	static bool isUnderAttackOf(const Board& board, const sf::Vector2i& target, const Figure::Color color);
	static bool isPat(const Board& board, const Figure::Color color);
	static bool isMat(Board& board, const Figure::Color color);


	static sf::Vector2i findKingOnBoard(const Board& board, const Figure::Color color);
};


template<typename... Lambdas>
struct Overload : Lambdas... {
	using Lambdas::operator()...;
};

namespace Movements {
	struct Illegal {};
	struct Common {};
	struct Castling {};
	struct EnPassan {};
}

using Movement = std::variant<Movements::Illegal, Movements::Common, Movements::Castling, Movements::EnPassan>;
