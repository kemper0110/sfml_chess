#include "h2hChess.h"

#include "Figures.h"
#include "Board.h"
#include "FigureMoves.h"

#include "Endgame.h"

h2hChess::h2hChess(Engine& engine) : Frame(engine), board(Figure::Color::White)
{

	movement_color = sf::Color(40, 255, 40, 150);
	selected_color = sf::Color(40, 40, 200, 150);
	cs = sf::CircleShape(7, 15);
	cs.setFillColor(movement_color);
	cs.setOrigin(sf::Vector2f(cs.getRadius(), cs.getRadius()));

	//engine.invalidate_rect();

}

void h2hChess::process_event(sf::Event event)
{
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		const auto click = event.mouseButton;
		switch (click.button) {
		case sf::Mouse::Left:
			const sf::Vector2f clickpos = sf::Vector2f(static_cast<float>(click.x), static_cast<float>(click.y));
			const auto clickidx = board.playing_as == Figure::Color::White ? sf::Vector2i(clickpos / 56.f) : sf::Vector2i(7, 7) - sf::Vector2i(clickpos / 56.f);

			if (clickidx.x < 0 or clickidx.y < 0 or clickidx.x > 7 or clickidx.y > 7) break;

			const auto& clicked_figure = board.at(clickidx);
			if (selected) {
				const auto& selected_figure = board.at(selected.value());

				if (clicked_figure and
					clicked_figure->getColor() == selected_figure->getColor() and
					not std::holds_alternative<Movements::Castling>(selected_figure->canMove(clickidx)))
					selected.emplace(clickidx);
				else if (not std::holds_alternative<Movements::Illegal>(selected_figure->canMove(clickidx)) and clickidx != selected.value()) {
					board.move(selected.value(), clickidx);
					selected.reset();
					board.who_moves = board.who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;

					if (FigureMoves::isPat(board, board.who_moves)) {
						std::cout << "Pat for " << static_cast<int>(board.who_moves) << '\n';
						engine.set_frame(std::make_shared<Endgame>(engine, false));
						return;
					}
					if (FigureMoves::isMat(board, board.who_moves)) {
						std::cout << "Mat for " << static_cast<int>(board.who_moves) << '\n';
						engine.set_frame(std::make_shared<Endgame>(engine, board.playing_as != board.who_moves));
						return;
					}

				}
			}
			else {
				if (clicked_figure and board.who_moves == clicked_figure->getColor())
					selected = clickidx;
			}
			engine.invalidate_rect();

			break;
		}
		break;
	}
}

void h2hChess::render()
{
	engine.window.draw(board);
	if (selected) {
		cs.setFillColor(selected_color);
		cs.setPosition(sf::Vector2f(board.playing_as == Figure::Color::White ? selected.value() : sf::Vector2i{ 7, 7 } - selected.value()) * 56.f + sf::Vector2f(28.f, 28.f));
		engine.window.draw(cs);
		cs.setFillColor(movement_color);

		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j) {
				const auto& figure = board.at(selected.value());
				if (not std::holds_alternative<Movements::Illegal>(figure->canMove({ i, j }))) {
					cs.setPosition(sf::Vector2f(board.playing_as == Figure::Color::White ? sf::Vector2i{ i, j } : sf::Vector2i{ 7 - i, 7 - j }) * 56.f + sf::Vector2f(28.f, 28.f));
					engine.window.draw(cs);
				}
			}

	}
}
