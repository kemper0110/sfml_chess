#pragma once
namespace sf {
	class Event;
}

class Frame
{
protected:
	class Engine& engine;
public:
	Frame(class Engine& engine);
	virtual void process_event(sf::Event event) = 0;
	virtual void render() = 0;
	virtual ~Frame() = default;
};

