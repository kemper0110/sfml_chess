#include "OnlineChess.h"

#include "Board.h"
#include "FigureMoves.h"

#include "Endgame.h"

#include <queue>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

OnlineChess::OnlineChess(Engine& engine) : Frame(engine), ws(net::make_strand(ioc))
{


	movement_color = sf::Color(40, 255, 40, 150);
	selected_color = sf::Color(40, 40, 200, 150);
	cs = sf::CircleShape(7, 15);
	cs.setFillColor(movement_color);
	cs.setOrigin(sf::Vector2f(cs.getRadius(), cs.getRadius()));

	{
		tcp::resolver resolver(net::make_strand(ioc));
		auto results = resolver.resolve("localhost", "80");
		//auto results = resolver.resolve("localhost", "80");
		beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));
		auto endpoint = beast::get_lowest_layer(ws).connect(results);
	}
	beast::get_lowest_layer(ws).expires_never();
	ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
	ws.set_option(websocket::stream_base::decorator(
		[](websocket::request_type& req)
		{
			req.set(http::field::user_agent,
				std::string(BOOST_BEAST_VERSION_STRING) +
				" websocket-client-async");
		}));
	ws.handshake("localhost:80", "/");
	std::cout << "reading...\n";

	do {
		ws.read(buffer);
		auto data = buffer.data();
		const auto message = std::string_view((char*)data.data(), data.size());
		std::cout << "received: " << message << '\n';
		if (message[0] == 'S')
			switch (message[1]) {
			case 'W': board = std::make_unique<Board>(engine, Figure::Color::White); break;
			case 'B': board = std::make_unique<Board>(engine, Figure::Color::Black); break;
			}
		buffer.clear();
	} while (not board);
	std::cout << "match started\n";

	ws.async_read(buffer, beast::bind_front_handler(&OnlineChess::on_read, this));

	websocket_thread = std::thread([this] { ioc.run(); });
	websocket_thread.detach();


	//engine.invalidate_rect();
}



void OnlineChess::on_read(beast::error_code ec, size_t transfered) {

	auto data = buffer.data();
	const auto message = std::string_view((char*)data.data(), data.size());
	std::cout << "received: " << message << '\n';
	if (message[0] == 'M') {
		sf::Vector2i from = { message[1] - '0', message[2] - '0' };
		sf::Vector2i to = { message[3] - '0', message[4] - '0' };
		board->move(from, to);
		board->who_moves = board->who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;

		if (FigureMoves::isPat(*board, board->who_moves)) {
			std::cout << "Pat for " << static_cast<int>(board->who_moves) << '\n';
			engine.set_frame(std::make_shared<Endgame>(engine, false));
			return;
		}
		if (FigureMoves::isMat(*board, board->who_moves)) {
			std::cout << "Mat for " << static_cast<int>(board->who_moves) << '\n';
			engine.set_frame(std::make_shared<Endgame>(engine, board->playing_as != board->who_moves));
			return;
		}
		engine.invalidate_rect();
	}
	buffer.clear();
	ws.async_read(buffer, beast::bind_front_handler(&OnlineChess::on_read, this));
}

void OnlineChess::process_event(sf::Event event)
{
	if (not board)
		return;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		const auto click = event.mouseButton;
		switch (click.button) {
		case sf::Mouse::Left:
			if (board->who_moves != board->playing_as)
				break;
			const sf::Vector2f clickpos = sf::Vector2f(static_cast<float>(click.x), static_cast<float>(click.y));
			const auto clickidx = board->playing_as == Figure::Color::White ? sf::Vector2i(clickpos / 56.f) : sf::Vector2i(7, 7) - sf::Vector2i(clickpos / 56.f);

			if (clickidx.x < 0 or clickidx.y < 0 or clickidx.x > 7 or clickidx.y > 7) break;

			const auto& clicked_figure = board->at(clickidx);
			if (selected) {
				const auto& selected_figure = board->at(selected.value());

				if (clicked_figure and
					clicked_figure->getColor() == selected_figure->getColor() and
					not std::holds_alternative<Movements::Castling>(selected_figure->canMove(clickidx)))
					selected.emplace(clickidx);
				else if (not std::holds_alternative<Movements::Illegal>(selected_figure->canMove(clickidx)) and clickidx != selected.value()) {
					board->move(selected.value(), clickidx);
					{
						const auto& vec = selected.value();
						std::string message = (std::stringstream() << "M" << vec.x << vec.y << clickidx.x << clickidx.y).str();
						ws.async_write(net::buffer(message), [](beast::error_code ec, size_t) { if (ec) std::cerr << ec.what(); });
					}
					selected.reset();
					board->who_moves = board->who_moves == Figure::Color::Black ? Figure::Color::White : Figure::Color::Black;
					if (FigureMoves::isPat(*board, board->who_moves)) {
						std::cout << "Pat for " << static_cast<int>(board->who_moves) << '\n';
						engine.set_frame(std::make_shared<Endgame>(engine, false));
						return;
					}
					if (FigureMoves::isMat(*board, board->who_moves)) {
						std::cout << "Mat for " << static_cast<int>(board->who_moves) << '\n';
						engine.set_frame(std::make_shared<Endgame>(engine, board->playing_as != board->who_moves));
						return;
					}

				}
			}
			else {
				if (clicked_figure and board->who_moves == clicked_figure->getColor())
					selected = clickidx;
			}
			engine.invalidate_rect();

			break;
		}
		break;
	}
}

void OnlineChess::render()
{
	if (not board)
		return;
	engine.window.draw(*board);
	if (selected) {
		cs.setFillColor(selected_color);
		cs.setPosition(sf::Vector2f(board->playing_as == Figure::Color::White ? selected.value() : sf::Vector2i{ 7, 7 } - selected.value()) * 56.f + sf::Vector2f(28.f, 28.f));
		engine.window.draw(cs);
		cs.setFillColor(movement_color);

		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j) {
				const auto& figure = board->at(selected.value());
				if (not std::holds_alternative<Movements::Illegal>(figure->canMove({ i, j }))) {
					cs.setPosition(sf::Vector2f(board->playing_as == Figure::Color::White ? sf::Vector2i{ i, j } : sf::Vector2i{ 7 - i, 7 - j }) * 56.f + sf::Vector2f(28.f, 28.f));
					engine.window.draw(cs);
				}
			}

	}
}

OnlineChess::~OnlineChess()
{
	ioc.stop();
}
