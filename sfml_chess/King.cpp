#include "King.h"
#include "FigureMoves.h"

#include "Board.h"


King::King(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::King, color, pos, board) {}

std::unique_ptr<Figure> King::clone() const {
	return std::make_unique<King>(this->getColor(), getPosition(), board);
}
bool King::canMove(sf::Vector2i newpos) const {
	if (not Figure::canMove(newpos) or not canAttack(newpos))
		return false;


	//const auto& figure = board.at(newpos);
	Figure::Color color;
	switch (getColor()) {
	case Figure::Color::Black: color = Figure::Color::White; break;
	case Figure::Color::White: color = Figure::Color::Black; break;
	default: throw;
	}

	if (FigureMoves::isUnderAttackOf(board, newpos, color))
		return false;
	return true;
}
bool King::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;
	if (
		newpos.y == pos.y and std::abs(newpos.x - pos.x) == 1	// vertical step
		or
		newpos.x == pos.x and std::abs(newpos.y - pos.y) == 1	// horizontal step
		or
		std::abs(newpos.x - pos.x) == 1 and std::abs(newpos.y - pos.y) == 1		// diagonal step
		)
	{
		return true;
	}
	return false;
}
void King::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));

	switch (getColor()) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 4, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 4, 56, 56, 56));
		break;
	}
	target.draw(sprite, states);
}