#include "King.h"
#include "FigureMoves.h"
#include "Board.h"


King::King(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::King, color, pos, 'K', board) {}

std::unique_ptr<Figure> King::clone() const {
	return std::make_unique<King>(this->getColor(), getPosition(), board);
}
Movement King::canMove(sf::Vector2i newpos) {
	bool color_flag;
	Figure::Color opposite_color;
	switch (getColor()) {
	case Figure::Color::White: color_flag = 0; opposite_color = Figure::Color::Black; break;
	case Figure::Color::Black: color_flag = 1; opposite_color = Figure::Color::White; break;
	default: throw;
	}

	const auto can_castle = std::invoke([this, newpos, color_flag, opposite_color] {
		//																								WHITE = 0		BLACK = 1
		if (FigureMoves::isUnderAttackOf(board, FigureMoves::findKingOnBoard(board, getColor()), opposite_color))
			return false;

		if (color_flag) {	// black
			if (pos != sf::Vector2i(4, 0))
				return false;

			if (newpos != sf::Vector2i(7, 0) and newpos != sf::Vector2i(0, 0))
				return false;

			if (newpos == sf::Vector2i(7, 0)) {	// short castling
				if (board.at({ 5, 0 }) or board.at({ 6, 0 }))
					return false;
				if (FigureMoves::isUnderAttackOf(board, sf::Vector2i(5, 0), opposite_color) or
					FigureMoves::isUnderAttackOf(board, sf::Vector2i(6, 0), opposite_color))
					return false;
				// checking history
				if (not board.getHistory().empty())
					for (const auto& step : board.getHistory()) 
						if (
						const auto part = std::string_view(step | std::views::drop_while([](char x) { return x != '|'; }) | std::views::drop(1) | std::views::take(3));
						part == "K47" or part == "R77")
							return false;
				return true;
			}
			else {	// long castling
				if (board.at({ 1, 0 }) or board.at({ 2, 0 }) or board.at({ 3, 0 }))
					return false;
				if (FigureMoves::isUnderAttackOf(board, sf::Vector2i(2, 0), opposite_color) or
					FigureMoves::isUnderAttackOf(board, sf::Vector2i(3, 0), opposite_color))
					return false;
				// checking history
				if (not board.getHistory().empty())
					for (const auto& step : board.getHistory()) 
						if (
						const auto part = std::string_view(step | std::views::drop_while([](char x) { return x != '|'; }) | std::views::drop(1) | std::views::take(3));
							part == "K47" or part == "R07")
							return false;
				return true;
			}
		}
		else {		// white

			if (pos != sf::Vector2i(4, 7))
				return false;

			if (newpos != sf::Vector2i(7, 7) and newpos != sf::Vector2i(0, 7))
				return false;

			if (newpos == sf::Vector2i(7, 7)) {	// short castling
				if (board.at({ 5, 7 }) or board.at({ 6, 7 }))
					return false;
				if (FigureMoves::isUnderAttackOf(board, sf::Vector2i(5, 7), opposite_color) or
					FigureMoves::isUnderAttackOf(board, sf::Vector2i(6, 7), opposite_color))
					return false;
				// checking history
				if (not board.getHistory().empty())
					for (const auto& step : board.getHistory())
						if (const auto part = std::string_view(step.begin(), step.begin() + 3);
							part == "K40" or part == "R70")
							return false;
				return true;
			}
			else {	// long castling
				if (board.at({ 1, 7 }) or board.at({ 2, 7 }) or board.at({ 3, 7 }))
					return false;
				if (FigureMoves::isUnderAttackOf(board, sf::Vector2i(2, 7), opposite_color) or
					FigureMoves::isUnderAttackOf(board, sf::Vector2i(3, 7), opposite_color))
					return false;
				// checking history
				if (not board.getHistory().empty())
					for (const auto& step : board.getHistory())
						if (const auto part = std::string_view(step.begin(), step.begin() + 3);
							part == "K40" or part == "R00")
							return false;
				return true;
			}

		}

		return false;
		}
	);
	if (can_castle)
		return Movements::Castling{};


	if (not std::holds_alternative<Movements::Common>(Figure::canMove(newpos)) or not canAttack(newpos))
		return Movements::Illegal{};

	//if (FigureMoves::isUnderAttackOf(board, newpos, opposite_color))
	//	return Movements::Illegal{};

	return Movements::Common{};
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
