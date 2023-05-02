#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include <future>
#include <iostream>
#include <memory>

class Engine
{
	std::mutex frame_mutex;
	std::shared_ptr<class Frame> frame;

	std::thread render_thread;
	std::atomic_bool rect_invalidated = true;
	std::mutex render_mutex;
	std::condition_variable render_cv;


public:
	sf::RenderWindow window;

	Engine();
	int run();
	void loop();
	void async_rendering();
	void invalidate_rect();
	void set_frame(std::shared_ptr<class Frame>&& frame);

	// resources
	std::shared_ptr<sf::Font> font;
	sf::Sprite board_sprite;
	sf::Texture board_texture;
	sf::Sprite figure_sprite;
	sf::Texture figure_texture;
};

