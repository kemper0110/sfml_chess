//#include "Figures.h"
#include "Board.h"
#include "FigureMoves.h"
#include <sfml/Graphics.hpp>
#include <iostream>
#include <optional>
#include <chrono>




int main()
{
	sf::RenderWindow window(sf::VideoMode(450, 450), "");
	window.setFramerateLimit(10);


	//const sf::Vector2f board_offset(28, 28);

	Board board(Figure::Color::White);

	std::optional<sf::Vector2i> selected;

	sf::Color movement_color(40, 255, 40, 150), selected_color(40, 40, 200, 150);
	sf::CircleShape cs(7, 15);
	cs.setFillColor(movement_color);
	cs.setOrigin(sf::Vector2f(cs.getRadius(), cs.getRadius()));


	window.clear();
	window.draw(board);
	window.display();

	while (window.isOpen()) {
		sf::Event event;
		if (window.waitEvent(event))
			do {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					}
					break;
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

							if (clicked_figure and clicked_figure->getColor() == selected_figure->getColor())
								selected.emplace(clickidx);
							else if (not std::holds_alternative<Movements::Illegal>(selected_figure->canMove(clickidx)) and clickidx != selected.value()) {
								board.move(selected.value(), clickidx);
								selected.reset();
								board.who_moves = board.who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;

								if (FigureMoves::isPat(board, board.who_moves)) {
									std::cout << "Pat for " << static_cast<int>(board.who_moves) << '\n';
									window.close();
								}
								if (FigureMoves::isMat(board, board.who_moves)) {
									std::cout << "Mat for " << static_cast<int>(board.who_moves) << '\n';
									window.close();
								}

								// если нет ходов у фигур => ПАТ
								// проверить наличие ШАХА королю => следующий шаг должен предотвратить ШАХ
								// если таких шагов нет => МАТ
							}
						}
						else {
							if (clicked_figure and board.who_moves == clicked_figure->getColor())
								selected = clickidx;
						}
						const auto end = std::chrono::high_resolution_clock::now();

						// drawing
						window.clear();
						window.draw(board);
						if (selected) {
							cs.setFillColor(selected_color);
							cs.setPosition(sf::Vector2f(board.playing_as == Figure::Color::White ?  selected.value() : sf::Vector2i{7, 7} - selected.value()) * 56.f + sf::Vector2f(28.f, 28.f));
							window.draw(cs);
							cs.setFillColor(movement_color);

							for (int i = 0; i < 8; ++i)
								for (int j = 0; j < 8; ++j) {
									const auto& figure = board.at(selected.value());
									const auto res = figure->canMove({ i, j });
									const auto valid = not std::holds_alternative<Movements::Illegal>(res);
									if (valid) {
										cs.setPosition(sf::Vector2f(board.playing_as == Figure::Color::White ? sf::Vector2i{ i, j } : sf::Vector2i{ 7 - i, 7 - j }) * 56.f + sf::Vector2f(28.f, 28.f));
										window.draw(cs);
									}
								}

						}
						window.display();

						break;
					}
					break;
				}
			} while (window.pollEvent(event));
	}
}
