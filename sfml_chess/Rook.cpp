#include "Rook.h"
#include "FigureMoves.h"



Rook::Rook(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Rook, color, pos, board) {
	sprite.setTexture(Figure::texture);
	switch (color) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 0, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 0, 56, 56, 56));
		break;
	}
}

std::unique_ptr<Figure> Rook::clone() const {
	return std::make_unique<Rook>(this->getColor(), getPosition(), board);
}
bool Rook::canMove(sf::Vector2i newpos) const {
	return Figure::canMove(newpos) and this->canAttack(newpos);
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
	target.draw(sprite, states);

}
