#include "Endgame.h"
#include "Menu.h"
#include "Engine.h"


Endgame::Endgame(Engine& engine, bool win) : Frame(engine), win(win)
{

	rect = sf::RectangleShape(sf::Vector2f(engine.window.getSize()));
	rect.setFillColor(sf::Color(50, 50, 50));

	menu_button = sf::RectangleShape(sf::Vector2f(250.f, 50.f));

	// set at center of window
	menu_button.setPosition(sf::Vector2f(engine.window.getSize() / 2u) - menu_button.getSize() / 2.f + sf::Vector2f(0.f, 100.f));

	menu_button.setFillColor(sf::Color(30, 30, 30));
	menu_button.setOutlineColor(sf::Color(100, 100, 100));
	menu_button.setOutlineThickness(1.f);

	menu_text = sf::Text("return to menu", *engine.font.get(), 20U);
	menu_text.setFillColor(sf::Color::White);
	// set at center of button
	{
		const auto text_bounds = menu_text.getGlobalBounds();
		const auto text_size = sf::Vector2f{ text_bounds.width, text_bounds.height };
		menu_text.setPosition(menu_button.getPosition() + menu_button.getSize() / 2.f - text_size / 2.f);
	}

	sentence_text = sf::Text(win ? "Congratulations" : "Better luck next time", *engine.font.get(), 20U);
	sentence_text.setFillColor(sf::Color::White);
	// set at center of screen
	{
		const auto text_bounds = menu_text.getGlobalBounds();
		const auto text_size = sf::Vector2f{ text_bounds.width, text_bounds.height };
		sentence_text.setPosition(sf::Vector2f(engine.window.getSize() / 2u) - text_size / 2.f);
	}

	//engine.invalidate_rect();

}

void Endgame::process_event(sf::Event event)
{
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left: {
			const auto click_pos = sf::Vector2i{ event.mouseButton.x, event.mouseButton.y };
			if (menu_button.getGlobalBounds().contains(sf::Vector2f(click_pos))) {
				engine.set_frame(std::make_shared<Menu>(engine));
				return;
			}
			break;
		}
		}
		break;
	}
}

void Endgame::render()
{
	engine.window.draw(rect);
	engine.window.draw(menu_button);
	engine.window.draw(menu_text);
	engine.window.draw(sentence_text);
}
