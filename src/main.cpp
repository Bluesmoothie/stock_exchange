#include "sotck_exchange.hpp"

int main(int, char**)
{
	stockExchange	StockExchange;
	Video			video([&StockExchange](){StockExchange.draw();});

    if (video.init()) {
		return 1;
	}

	video.loop();

    return 0;
}