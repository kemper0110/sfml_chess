#include "Bishop.h"

#include "FigureMoves.h"


Bishop::Bishop(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Bishop, color, pos, board) {
	sprite.setTexture(Figure::texture);
	switch (color) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 2, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 2, 56, 56, 56));
		break;
	}
}

std::unique_ptr<Figure> Bishop::clone() const {
	return std::make_unique<Bishop>(this->getColor(), getPosition(), board);
}
bool Bishop::canMove(sf::Vector2i newpos) const {
	return Figure::canMove(newpos) and this->canAttack(newpos);

}
bool Bishop::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;

	if (std::abs(pos.x - newpos.x) == std::abs(pos.y - newpos.y) and FigureMoves::isDiagonalFree(board, pos, newpos))
		return true;
	return false;
}
void Bishop::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);

}
