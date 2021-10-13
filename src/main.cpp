#include "include/game.hpp"

int run(Game* game) {
	game->Init();
	while (game->Tick()) {}
	delete game;
	return 0;
}

int main()
{
	return run(new Game());
}