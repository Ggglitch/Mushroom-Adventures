#include "pch.h"
#include "Game.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(0));

	Game game;
	game.loop();

	return 0;
}