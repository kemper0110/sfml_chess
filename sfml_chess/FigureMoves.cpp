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
	const auto king = FigureMoves::findKingOnBoard(board, color);

	Figure::Color opposite_color;
	switch (color) {
	case Figure::Color::White: opposite_color = Figure::Color::Black; break;
	case Figure::Color::Black: opposite_color = Figure::Color::White; break;
	default: throw;
	}
	if (FigureMoves::isUnderAttackOf(board, king, opposite_color))
		return false;

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

bool FigureMoves::isMat(Board& board, const Figure::Color color)
{


	auto& data = board.getData();
	const auto king = FigureMoves::findKingOnBoard(board, color);


	Figure::Color opposite_color;
	switch (color) {
	case Figure::Color::Black: opposite_color = Figure::Color::White; break;
	case Figure::Color::White: opposite_color = Figure::Color::Black; break;
	default: throw;
	}

	if (not FigureMoves::isUnderAttackOf(board, king, opposite_color))
		return false;

	for (auto& row : data) {
		for (auto& figure : row) {
			// skip cells without figures or figures with oppposite color
			if (not figure or figure->getColor() == opposite_color)
				continue;
			for (int y = 0; y < data.size(); ++y)
				for (int x = 0; x < data[y].size(); ++x)
					// check cells where figure can move
					if (figure->canMove({ x, y })) {

						// moving figure
						const auto prev_pos = figure->getPosition();
						figure->move({ x, y });
						auto& target = data[y][x];
						auto prev_cell = std::move(target);
						target = std::move(figure);

						const auto bad_move = FigureMoves::isUnderAttackOf(board, king, opposite_color);

						// returning figure to previous pos
						figure = std::move(target);
						target = std::move(prev_cell);
						figure->move(prev_pos);

						if (not bad_move)
							return false;
					}
		}
	}
	return true;
}

sf::Vector2i FigureMoves::findKingOnBoard(const Board& board, const Figure::Color color)
{
	for (const auto& row : board.getData())
		for (const auto& figure : row)
			if (figure and figure->getType() == Figure::Type::King and figure->getColor() == color)
				return figure->getPosition();

	throw "wtf where is king?";
}
