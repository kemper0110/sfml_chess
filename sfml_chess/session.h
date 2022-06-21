#pragma once
#define _WIN32_WINNT 0x0601

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

#include <SFML/System/Vector2.hpp>

#include "Board.h"

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


// Report a failure
void fail(beast::error_code ec, char const* what);


class session : public std::enable_shared_from_this<session>
{
	tcp::resolver resolver_;
	websocket::stream<beast::tcp_stream> ws_;
	beast::flat_buffer buffer_;

	std::shared_ptr<Board>& board;
	std::condition_variable& board_cv;
	std::mutex& board_mutex;

	std::condition_variable& render_cv;
	std::atomic_bool& re_render;

public:
	explicit session(net::io_context& ioc, std::shared_ptr<Board>& board_ptr, std::condition_variable& board_cv, 
		std::mutex& board_mutex, std::condition_variable& render_cv, std::atomic_bool& re_render);

	void send(std::string_view message);

	void on_write(beast::error_code ec, std::size_t bytes_transferred);

	void run(std::shared_ptr<Board>& ptr);

	void on_read(beast::error_code ec, size_t transfered);

	void handle_message(std::string_view message);
};
