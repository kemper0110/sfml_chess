#include "Engine.h"
//#include "Frame.h"

#include "Menu.h"

#include <future>
#include <iostream>

Engine::Engine() : window(sf::VideoMode(450, 450), "Chess")
{
	constexpr char board_filename[]{ "board0.png" };
	board_texture.loadFromFile(board_filename);
	board_sprite.setTexture(board_texture);

	constexpr char figure_filename[]{ "figures.png" };
	figure_texture.loadFromFile(figure_filename);
	figure_sprite.setTexture(figure_texture);

	font = std::make_shared<sf::Font>();
	if (!font->loadFromFile("Segoe ui bold.ttf"))
		std::cerr << "font not found\n";

	this->set_frame(std::make_shared<Menu>(*this));

	window.setActive(false);
}

int Engine::run()
{
	render_thread = std::thread(&Engine::async_rendering, this);
	loop();
	render_thread.join();	// maybe better to detach?
	return 0;
}

void Engine::loop()
{
	while (window.isOpen()) {
		sf::Event event;
		if (window.waitEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
			{
				std::unique_lock lk(render_mutex);
				window.close();
			}
			}
			frame->process_event(event);
		}
	}
}

void Engine::async_rendering()
{
	window.setActive(true);
	while (window.isOpen()) {
		std::unique_lock lk(render_mutex);
		render_cv.wait(lk, [this]() -> bool { return rect_invalidated; });

		window.clear();
		frame->render();
		window.display();
		rect_invalidated = false;
	}
}

void Engine::invalidate_rect()
{
	{
		std::unique_lock lk(render_mutex);
		rect_invalidated = true;
	}
	render_cv.notify_one();
}

void Engine::set_frame(std::shared_ptr<Frame>&& frame)
{
	{
		std::unique_lock wait_until_render_stops(render_mutex);
	}
	const auto _ = std::async(std::launch::async, &Engine::invalidate_rect, this);	// spurious magic for working from dying thread(e.g. websocket)
	this->frame = std::move(frame);
	//invalidate_rect();
}
