#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 10
#include <cstdlib>
#include "SFML/Graphics.hpp"

class Game {
private:
	int cell_width_ = 32;
	int hided_board[BOARD_SIZE][BOARD_SIZE];
	int game_board[BOARD_SIZE][BOARD_SIZE];
	sf::Texture texture;
	sf::Sprite sprite;

private:
	void InitBoard(); // Create board
	void CountMines(); // Count mines around cell

public:
	Game() = default;
	void Start(); // Start the game
};

#endif
