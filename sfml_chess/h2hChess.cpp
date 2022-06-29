#include "h2hChess.h"

#include "Figures.h"
#include "Board.h"
#include "FigureMoves.h"

#include "Endgame.h"

h2hChess::h2hChess(Engine& engine) : Frame(engine), board(engine, Figure::Color::White)
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


			if (board.promotion) {
				if (clickidx.x < 2 or clickidx.x > 5 or clickidx.y != 4)
					break;

				const auto pos = board.promotion.value();
				std::unique_ptr<Figure> figure;
				switch (clickidx.x) {
				case 2: figure = std::make_unique<Queen>(board.who_moves, pos, board); break;
				case 3: figure = std::make_unique<Rook>(board.who_moves, pos, board); break;
				case 4: figure = std::make_unique<Knight>(board.who_moves, pos, board); break;
				case 5: figure = std::make_unique<Bishop>(board.who_moves, pos, board); break;
				default: throw;
				}
				board.at(pos) = std::move(figure);
				board.who_moves = board.who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;
				board.promotion.reset();

				engine.invalidate_rect();
				break;
			}

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

					if (not board.promotion)
						board.who_moves = board.who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;
					selected.reset();

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

	if (board.promotion) {
		// draw 4 figures on display
		// Q R N B
		sf::RectangleShape rect(sf::Vector2f(engine.window.getSize()));
		rect.setFillColor(sf::Color(0, 0, 0, 140));
		engine.window.draw(rect);
		constexpr auto tex_idx_y = std::array{ 0, 1 };
		constexpr auto tex_idx_x = std::array{ 3, 0, 1, 2 };


		bool color_flag;
		switch (board.who_moves) {
		case Figure::Color::Black: color_flag = 0; break;
		case Figure::Color::White: color_flag = 1; break;
		default: throw;
		}

		sf::Vector2i pos = { 2, 4 };
		for (const auto x : tex_idx_x) {
			engine.figure_sprite.setTextureRect(sf::IntRect(56 * x, tex_idx_y[color_flag] * 56, 56, 56));
			engine.figure_sprite.setPosition(sf::Vector2f(pos * 56));
			engine.window.draw(engine.figure_sprite);
			pos.x++;
		}
		return;
	}

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
		return;
	}
}
