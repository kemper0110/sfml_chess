//#include "Figures.h"
#include "Board.h"
#include "FigureMoves.h"
#include <sfml/Graphics.hpp>
#include <iostream>
#include <optional>
#include <chrono>




int main()
{
	sf::RenderWindow window(sf::VideoMode(504, 504), "");
	window.setFramerateLimit(10);


	//const sf::Vector2f board_offset(28, 28);

	Board board;

	std::optional<sf::Vector2i> selected;

	auto who_moves = Figure::Color::White;


	sf::Color movement_color(40, 255, 40, 150), selected_color(40, 40, 200, 150);
	sf::CircleShape cs(7, 15);
	cs.setFillColor(movement_color);
	cs.setOrigin(sf::Vector2f(cs.getRadius(), cs.getRadius()) - Figure::board_offset);

	while (window.isOpen()) {
		window.clear();

		window.draw(board);

		if (selected) {
			cs.setFillColor(selected_color);
			cs.setPosition(Figure::board_offset + sf::Vector2f(selected.value()) * 56.f);
			window.draw(cs);
			cs.setFillColor(movement_color);

			for (int i = 0; i < 8; ++i)
				for (int j = 0; j < 8; ++j)
					if (board.at(selected.value())->canMove({ i, j })) {
						cs.setPosition(Figure::board_offset + sf::Vector2f(static_cast<float>(i), static_cast<float>(j)) * 56.f);
						window.draw(cs);
					}
		}

		window.display();


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

						const auto begin = std::chrono::high_resolution_clock::now();


						const sf::Vector2f clickpos = sf::Vector2f(static_cast<float>(click.x), static_cast<float>(click.y)) - Figure::board_offset;
						const auto clickidx = sf::Vector2i(clickpos / 56.f);

						if (clickidx.x < 0 or clickidx.y < 0 or clickidx.x > 7 or clickidx.y > 7) break;

						const auto& clicked_figure = board.at(clickidx);
						if (selected) {
							const auto& selected_figure = board.at(selected.value());

							if (clicked_figure and clicked_figure->getColor() == selected_figure->getColor())
								selected.emplace(clickidx);
							else if (selected_figure->canMove(clickidx) and clickidx != selected.value()) {
								board.move(selected.value(), clickidx);
								selected.reset();
								who_moves = who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;

								if (FigureMoves::isPat(board, who_moves)) {
									std::cout << "Pat for " << static_cast<int>(who_moves) << '\n';
									window.close();
								}
								if (FigureMoves::isMat(board, who_moves)) {
									std::cout << "Mat for " << static_cast<int>(who_moves) << '\n';
									window.close();
								}

								// если нет ходов у фигур => ПАТ
								// проверить наличие ШАХА королю => следующий шаг должен предотвратить ШАХ
								// если таких шагов нет => МАТ
							}
						}
						else {
							if (clicked_figure and who_moves == clicked_figure->getColor())
								selected = clickidx;
						}
						const auto end = std::chrono::high_resolution_clock::now();

						std::cout << "time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin) << '\n';

						break;
					}
					break;
				}
			} while (window.pollEvent(event));
	}
}
