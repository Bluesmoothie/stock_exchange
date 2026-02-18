#include "sotck_exchange.hpp"

int main(int, char**)
{
	Video	video;

    if (video.init()) {
		return 1;
	}

	video.loop();

    return 0;
}