#pragma once
#include "Frame.h"
#include "Board.h"

//#include "session.h"

#include <sdkddkver.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

namespace beast = boost::beast;         
namespace http = beast::http;           
namespace websocket = beast::websocket; 
namespace net = boost::asio;            
using tcp = boost::asio::ip::tcp;

class OnlineChess : public Frame
{
	std::shared_ptr<Board> board;
	std::optional<sf::Vector2i> selected;
	sf::Color movement_color, selected_color;
	sf::CircleShape cs;

	std::thread websocket_thread;

	net::io_context ioc;
	websocket::stream<beast::tcp_stream> ws;
	beast::flat_buffer buffer;

public:
	OnlineChess(class Engine& engine);


	// Унаследовано через Frame
	virtual void process_event(sf::Event event) override;

	virtual void render() override;

	void on_read(beast::error_code ec, size_t transfered);

	~OnlineChess();
};

