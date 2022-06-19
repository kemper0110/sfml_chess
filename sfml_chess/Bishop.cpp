#include "Bishop.h"
#include "FigureMoves.h"
#include "Board.h"


Bishop::Bishop(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Bishop, color, pos, 'B', board) {}

std::unique_ptr<Figure> Bishop::clone() const {
	return std::make_unique<Bishop>(this->getColor(), getPosition(), board);
}
Movement Bishop::canMove(sf::Vector2i newpos) {
	if (std::holds_alternative<Movements::Common>(Figure::canMove(newpos)) and canAttack(newpos))
		return Movements::Common{};
	else
		return Movements::Illegal{};
}
bool Bishop::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;

	if (std::abs(pos.x - newpos.x) == std::abs(pos.y - newpos.y) and FigureMoves::isDiagonalFree(board, pos, newpos))
		return true;
	return false;
}

