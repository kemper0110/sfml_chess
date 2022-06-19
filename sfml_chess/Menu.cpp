#include "Menu.h"
//#include "Frame.h"

#include "h2hChess.h"


Menu::Menu(Engine& engine, std::shared_ptr<sf::Font> font) : Frame(engine)
{
	rect = sf::RectangleShape(sf::Vector2f(engine.window.getSize()));
	rect.setFillColor(sf::Color(50, 50, 50));

	button1 = sf::RectangleShape(sf::Vector2f(250.f, 50.f));

	// set at center of window
	button1.setPosition(sf::Vector2f(engine.window.getSize() / 2u) - button1.getSize() / 2.f);

	button1.setFillColor(sf::Color(30, 30, 30));
	button1.setOutlineColor(sf::Color(100, 100, 100));
	button1.setOutlineThickness(1.f);

	text = sf::Text("play from hand to hand", *font.get(), 20U);
	/*text.setString("play from hand to hand");
	text.setCharacterSize(20U);*/
	text.setFillColor(sf::Color::White);
	// set at center of button
	const auto text_bounds = text.getGlobalBounds();
	const auto text_size = sf::Vector2f{ text_bounds.width, text_bounds.height };
	text.setPosition(button1.getPosition() + button1.getSize() / 2.f - text_size / 2.f);
}

void Menu::process_event(sf::Event event)
{
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left: {
			const auto click_pos = sf::Vector2i{ event.mouseButton.x, event.mouseButton.y };
			if (button1.getGlobalBounds().contains(sf::Vector2f(click_pos))) {
				engine.set_frame(std::make_shared<h2hChess>(engine));
				return;
			}
			break;
		}
		}
		break;
	case sf::Event::KeyPressed:
		switch (event.key.code) {
		case sf::Keyboard::Left:
			color = sf::Color::Blue;
			engine.invalidate_rect();
			break;
		case sf::Keyboard::Right:
			color = sf::Color::Red;
			engine.invalidate_rect();
			break;
		}
		break;
	}
}

void Menu::render()
{
	engine.window.draw(rect);
	engine.window.draw(button1);
	engine.window.draw(text);
}

