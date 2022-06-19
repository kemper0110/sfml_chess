#include "Engine.h"
//#include "Frame.h"

#include "Menu.h"



Engine::Engine() : window(sf::VideoMode(450, 450), "Chess")
{
	font = std::make_shared<sf::Font>();
	font->loadFromFile("Segoe ui bold.ttf");

	this->set_frame(std::make_shared<Menu>(*this, font));

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
	this->frame = std::move(frame);
	invalidate_rect();
}
