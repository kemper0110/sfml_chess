#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <atomic>



#include "Frame.h"


class Frame;


class Engine
{
	std::mutex frame_mutex;
	std::shared_ptr<Frame> frame;

	std::thread render_thread;
	std::atomic_bool rect_invalidated = true;
	std::mutex render_mutex;
	std::condition_variable render_cv;

	// resources
	std::shared_ptr<sf::Font> font;
public:
	sf::RenderWindow window;

	Engine();
	int run();
	void loop();
	void async_rendering();
	void invalidate_rect();
	void set_frame(std::shared_ptr<Frame>&& frame);
};

