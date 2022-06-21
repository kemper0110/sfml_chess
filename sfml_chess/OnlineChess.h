#pragma once
#include "Frame.h"
#include "Board.h"

//#include "session.h"

#define _WIN32_WINNT 0x0601

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

#include <queue>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

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
	OnlineChess(Engine& engine);


	// Унаследовано через Frame
	virtual void process_event(sf::Event event) override;

	virtual void render() override;

	void on_read(beast::error_code ec, size_t transfered);

	~OnlineChess();
};

