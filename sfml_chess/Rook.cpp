#include "Rook.h"
#include "FigureMoves.h"
#include "Board.h"


Rook::Rook(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Rook, color, pos, 'R', board) {}

std::unique_ptr<Figure> Rook::clone() const {
	return std::make_unique<Rook>(this->getColor(), getPosition(), board);
}
Movement Rook::canMove(sf::Vector2i newpos) {
	if (std::holds_alternative<Movements::Common>(Figure::canMove(newpos)) and this->canAttack(newpos))
		return Movements::Common{};
	else
		return Movements::Illegal{};
}
bool Rook::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;
	if (newpos.x == pos.x and FigureMoves::isHorizontalFree(board, pos, newpos)
		or
		newpos.y == pos.y and FigureMoves::isVerticalFree(board, pos, newpos))
		return true;
	return false;
}
void Rook::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));
	switch (getColor()) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 0, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 0, 56, 56, 56));
		break;
	}
	target.draw(sprite, states);
}
