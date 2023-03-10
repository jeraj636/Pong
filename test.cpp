#define STB_IMAGE_IMPLEMENTATION
#include "render.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include "net.h"
void netvorkF()
{
	sleep(2);
	char tip;
	io::vpis("TIP:", tip);
	if (tip == 'c')
	{
		net::Client client;
		io::vpis("IP:", client.ip);
		std::string sporocilo;
		std::array<char, 128> prejetoSporocilo;
		size_t len;
		while (1)
		{
			io::vpis("sporocilo:", sporocilo);
			client.posljiSporocilo(sporocilo);

			prejetoSporocilo = client.sprejmiSporocilo(len);
			std::cout.write(prejetoSporocilo.data(), len);
		}
	}
	else if (tip == 's')
	{
		net::Server server;
		std::string sporocilo;
		std::array<char, 128> prejetoSporocilo;
		size_t len;
		while (1)
		{
			prejetoSporocilo = server.sprejmiSporocilo(len);
			std::cout.write(prejetoSporocilo.data(), len);

			io::vpis("sporocilo:", sporocilo);
			server.posljiSporocilo(sporocilo);
		}
	}
}

int main()
{

	gl::Okno okno(900, 600);
	gl::Barva b(0x000000ff);
	gl::Barva ba(0x0000ffff);

	std::thread network(netvorkF);
	gl::GameObject kvadratD(600, 900);
	gl::Vec3<float> pozicija(0, 0, 0);

	while (!okno.aliMoramZapreti())
	{

		kvadratD.spremeniMiVelikost(gl::Vec3<float>(30, 120, 0));
		kvadratD.spremeniMiBarvo(ba);
		okno.pobarvaj(b);
		int input = okno.BeriInput();
		if (input == 1)
			pozicija.y += 0.03;
		if (input == 2)
			pozicija.y -= 0.03;
		kvadratD.premakniMe(pozicija);
		kvadratD.narisiMe();
		okno.osvezi();
	}
	network.detach();
}