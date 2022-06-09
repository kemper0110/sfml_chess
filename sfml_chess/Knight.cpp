#include "Knight.h"

Knight::Knight(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Knight, color, pos, board) {}

std::unique_ptr<Figure> Knight::clone() const {
	return std::make_unique<Knight>(this->getColor(), getPosition(), board);
}
bool Knight::canMove(sf::Vector2i newpos) const {
	return Figure::canMove(newpos) and this->canAttack(newpos);
}
bool Knight::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;

	const sf::Vector2i step1(1, 2), step2(2, 1);

	const sf::Vector2i diff = newpos - pos;
	const sf::Vector2i abs_diff = { std::abs(diff.x), std::abs(diff.y) };
	if (abs_diff == step1 or abs_diff == step2)
		return true;
	return false;
}
void Knight::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));

	switch (getColor()) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 1, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 1, 56, 56, 56));
		break;
	}
	target.draw(sprite, states);
}