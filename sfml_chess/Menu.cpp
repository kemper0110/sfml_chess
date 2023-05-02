#include "Menu.h"
//#include "Frame.h"

#include "h2hChess.h"
#include "OnlineChess.h"



Menu::Menu(Engine& engine) : Frame(engine)
{

	rect = sf::RectangleShape(sf::Vector2f(engine.window.getSize()));
	rect.setFillColor(sf::Color(50, 50, 50));

	h2h_button = sf::RectangleShape(sf::Vector2f(250.f, 50.f));

	// set at center of window
	h2h_button.setPosition(sf::Vector2f(engine.window.getSize() / 2u) - h2h_button.getSize() / 2.f - sf::Vector2f(0.f, 100.f));

	h2h_button.setFillColor(sf::Color(30, 30, 30));
	h2h_button.setOutlineColor(sf::Color(100, 100, 100));
	h2h_button.setOutlineThickness(1.f);

	h2h_text = sf::Text("play from hand to hand", *engine.font.get(), 20U);
	h2h_text.setFillColor(sf::Color::White);
	// set at center of button
	{
		const auto text_bounds = h2h_text.getGlobalBounds();
		const auto text_size = sf::Vector2f{ text_bounds.width, text_bounds.height };
		h2h_text.setPosition(h2h_button.getPosition() + h2h_button.getSize() / 2.f - text_size / 2.f);
	}


	online_button = sf::RectangleShape(sf::Vector2f(250.f, 50.f));
	online_button.setPosition(sf::Vector2f(engine.window.getSize() / 2u) - h2h_button.getSize() / 2.f + sf::Vector2f(0.f, 100.f));

	online_button.setFillColor(sf::Color(30, 30, 30));
	online_button.setOutlineColor(sf::Color(100, 100, 100));
	online_button.setOutlineThickness(1.f);

	online_text = sf::Text("play online", *engine.font.get(), 20U);
	online_text.setFillColor(sf::Color::White);
	// set at center of button
	{
		const auto text_bounds = online_text.getGlobalBounds();
		const auto text_size = sf::Vector2f{ text_bounds.width, text_bounds.height };
		online_text.setPosition(online_button.getPosition() + online_button.getSize() / 2.f - text_size / 2.f);
	}
	//engine.invalidate_rect();

}

void Menu::process_event(sf::Event event)
{
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left: {
			const auto click_pos = sf::Vector2i{ event.mouseButton.x, event.mouseButton.y };
			if (h2h_button.getGlobalBounds().contains(sf::Vector2f(click_pos))) {
				engine.set_frame(std::make_shared<h2hChess>(engine));
				return;
			}
			if (online_button.getGlobalBounds().contains(sf::Vector2f(click_pos))) {
				engine.set_frame(std::make_shared<OnlineChess>(engine));
				return;
			}
			break;
		}
		}
		break;
	}
}

void Menu::render()
{
	engine.window.draw(rect);

	engine.window.draw(h2h_button);
	engine.window.draw(h2h_text);

	engine.window.draw(online_button);
	engine.window.draw(online_text);


}

