#include "FigureMoves.h"
#include "Board.h"
#include "Vector2Utils.h"
#include "Figure.h"

bool FigureMoves::isDiagonalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst)
{
	const auto diff = dst - src;
	if (diff.x == 0 and diff.y == 0)
		return true;
	const auto dir = diff / std::abs(diff.x);

	for (auto vec = src + dir; vec != dst; vec += dir)
		if (board.at(vec))
			return false;

	return true;
}

bool FigureMoves::isVerticalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst)
{
	const auto diff = dst.x - src.x;
	if (diff == 0)
		return true;
	const auto step = diff / std::abs(diff);

	for (auto x = src.x + step; x != dst.x; x += step)
		if (board.at({ x, src.y }))
			return false;

	return true;
}

bool FigureMoves::isHorizontalFree(const Board& board, const sf::Vector2i& src, const sf::Vector2i& dst)
{
	const auto diff = dst.y - src.y;
	if (diff == 0)
		return true;
	const auto step = diff / std::abs(diff);

	for (auto y = src.y + step; y != dst.y; y += step)
		if (board.at({ src.x, y }))
			return false;

	return true;
}

bool FigureMoves::isKingShah(const Board& board, const sf::Vector2i& king)
{
	// if exist figure on board with opposite color that can move to king then king is on shah
	const auto& king_figure = board.at(king);
	for (const auto& row : board.getData())
		for (const auto& figure : row)
			if (figure->getColor() != king_figure->getColor() and figure->canMove(king))
				return true;

	return false;
}

bool FigureMoves::isUnderAttackOf(const Board& board, const sf::Vector2i& target, const Figure::Color color)
{



	for (const auto& row : board.getData())
		for (const auto& figure : row)
			if (figure and figure->getColor() == color and figure->canAttack(target))
				return true;

	return false;
}

bool FigureMoves::isPat(const Board& board, const Figure::Color color)
{
	const auto hasMoves = [&board](const auto& figure) {
		const auto& data = board.getData();
		for (int y = 0; y < data.size(); ++y)
			for (int x = 0; x < data[y].size(); ++x)
				if (figure->canMove({ y, x }))
					return true;
		return false;
	};

	for (const auto& row : board.getData())
		for (const auto& figure : row)
			if (figure and figure->getColor() == color)
				if (hasMoves(figure))
					return false;
	return true;
}
