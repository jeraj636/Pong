#pragma once

#include <array>
#include <asio.hpp>
#include <iostream>
namespace net
{
	class Client
	{
	public:
		std::string ip;
		Client()
			: resolver(ioContext), socket(ioContext)
		{
			ServerEndPoint =
				*resolver.resolve(asio::ip::udp::v4(), ip, "daytime").begin();
			socket.open(asio::ip::udp::v4());
		}
		void posljiSporocilo(std::string sporocilo)
		{
			socket.send_to(asio::buffer(sporocilo), ServerEndPoint);
		}

		std::array<char, 128> sprejmiSporocilo(size_t &len)
		{
			std::array<char, 128> sporocilo;
			asio::ip::udp::endpoint endPoint;
			len = socket.receive_from(asio::buffer(sporocilo), endPoint);
			return sporocilo;
		}

	private:
		asio::io_context ioContext;
		asio::ip::udp::resolver resolver;
		asio::ip::udp::endpoint ServerEndPoint;
		asio::ip::udp::socket socket;
	};
	class Server
	{
	public:
		Server()
			: socket(ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), 13))
		{
		}

		std::array<char, 128> sprejmiSporocilo(size_t &len)
		{
			asio::ip::udp::endpoint e;
			endPoint = e;
			std::array<char, 128> sporocilo;
			len = socket.receive_from(asio::buffer(sporocilo), endPoint);
			return sporocilo;
		}
		void posljiSporocilo(std::string sporocilo)
		{
			asio::error_code ignoredError;
			socket.send_to(asio::buffer(sporocilo), endPoint, 0, ignoredError);
		}

	private:
		asio::io_context ioContext;
		asio::ip::udp::socket socket;
		asio::ip::udp::endpoint endPoint;
	};
}
