#include "Queen.h"
#include "FigureMoves.h"
#include "Board.h"

Queen::Queen(Figure::Color color, sf::Vector2i pos, class Board& board) : Figure(Figure::Type::Queen, color, pos, 'Q', board) {}

std::unique_ptr<Figure> Queen::clone() const {
	return std::make_unique<Queen>(this->getColor(), getPosition(), board);
}
Movement Queen::canMove(sf::Vector2i newpos) {
	if (std::holds_alternative<Movements::Common>(Figure::canMove(newpos)) and this->canAttack(newpos))
		return Movements::Common{};
	else
		return Movements::Illegal{};
}
bool Queen::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;

	if (newpos.x == pos.x and FigureMoves::isVerticalFree(board, pos, newpos)
		or
		newpos.y == pos.y and FigureMoves::isHorizontalFree(board, pos, newpos))
		return true;
	
	if (const auto dv = pos - newpos;
		std::abs(dv.x) == std::abs(dv.y) and FigureMoves::isDiagonalFree(board, pos, newpos))
		return true;
	return false;
}
