#include "Queen.h"
#include "FigureMoves.h"
#include "Board.h"

Queen::Queen(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Queen, color, pos, 'Q', board) {}

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

	if (newpos.x == pos.x and FigureMoves::isHorizontalFree(board, pos, newpos)
		or
		newpos.y == pos.y and FigureMoves::isVerticalFree(board, pos, newpos))
		return true;

	if (std::abs(pos.x - newpos.x) == std::abs(pos.y - newpos.y) and FigureMoves::isDiagonalFree(board, pos, newpos))
		return true;
	return false;
}
void Queen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));

	switch (getColor()) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 3, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 3, 56, 56, 56));
		break;
	}
	target.draw(sprite, states);
}