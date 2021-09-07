#include <iostream>
#include "Game.h"

int main()
{
	std::cout << "WELCOME TO 5 CARD DRAW" << std::endl << std::endl;

	Game* poker = nullptr;

	while (poker->IsRunning())
	{
		poker = new Game();
		poker->Go();
		delete poker;
	}

	return 0;
}