#include "Game.h"

int main()
{

	srand(static_cast<unsigned>(time(NULL)));

	//Init game engine
	Game game;

	//Game loop
	while (game.running())
	{
		//update
		game.update();

		//render
		game.render();
	}

	return EXIT_SUCCESS;
}
