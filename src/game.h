#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 10
#include <cstdlib>

#include <SFML/Graphics.hpp>

class Game {
private:
	bool is_alive_;
	int cell_width_ = 32, flags_;
	int hided_board[BOARD_SIZE][BOARD_SIZE];
	int game_board[BOARD_SIZE][BOARD_SIZE];

private:
	void InitBoard(); // Create board
	void CountMines(); // Count mines around cell

public:
	Game() = default;
	void Start(); // Start the game
};

#endif
