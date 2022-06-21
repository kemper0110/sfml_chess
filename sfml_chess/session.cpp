#include "session.h"



void fail(beast::error_code ec, char const* what) {
	std::cerr << what << ": " << ec.message() << "\n";
}

session::session(net::io_context& ioc, std::shared_ptr<Board>& board_ptr, std::condition_variable& board_cv,
	std::mutex& board_mutex, std::condition_variable& render_cv, std::atomic_bool& re_render)
	: resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc)), board(board_ptr), board_cv(board_cv),
	board_mutex(board_mutex), render_cv(render_cv), re_render(re_render)
{}


void session::send(std::string_view message) {
	ws_.async_write(net::buffer(message), beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_write(
	beast::error_code ec,
	std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);
	if (ec)
		return fail(ec, "write");
}


void session::run(std::shared_ptr<Board>& ptr) {
	auto results = resolver_.resolve("localhost", "9001");
	beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
	auto endpoint = beast::get_lowest_layer(ws_).connect(results);
	beast::get_lowest_layer(ws_).expires_never();
	ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
	ws_.set_option(websocket::stream_base::decorator(
		[](websocket::request_type& req)
		{
			req.set(http::field::user_agent,
				std::string(BOOST_BEAST_VERSION_STRING) +
				" websocket-client-async");
		}));
	ws_.handshake("localhost:9001", "/");
	ws_.async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
	std::cout << "receiving started\n";
}



void session::on_read(beast::error_code ec, size_t transfered) {
	if (ec)
		return fail(ec, "read");
	const auto& buff = buffer_.data();
	handle_message({ (char*)buff.data(), buff.size() });
	buffer_.clear();

	ws_.async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
}



void session::handle_message(std::string_view message) {
	// M1234
	// from {1,2} move to {3,4}
	std::cout << "received: " << message << '\n';

	switch (message[0]) {
	case 'M':
	{
		sf::Vector2i from = { message[1] - '0', message[2] - '0' };
		sf::Vector2i to = { message[3] - '0', message[4] - '0' };
		// move figure
		{
			std::unique_lock lk(board_mutex);
			board->move(from, to);
		}
		// re_render board on screen
		re_render = true;
		render_cv.notify_one();
		std::cout << "notified to render\n";

		break;
	}
	case 'S':
		// game started
		switch (message[1]) {
		case 'W':
			// set side color to White 
			board = std::make_unique<Board>(Figure::Color::White);
			break;
		case 'B':
			// set side color to Black
			board = std::make_unique<Board>(Figure::Color::Black);
			break;
		}

		// and start game
		board_cv.notify_one();
		std::cout << "notified board creation\n";

		// re_render board on screen
		re_render = true;
		render_cv.notify_one();
		std::cout << "notified to render\n";

		break;
	}


}