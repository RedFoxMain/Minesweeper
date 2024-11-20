#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 10
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game {
private:
	bool is_alive_, win_;
	int cell_width_ = 32, flags_, guessed_positions_, bombs_;
	int hided_board[BOARD_SIZE][BOARD_SIZE];
	int game_board[BOARD_SIZE][BOARD_SIZE];

	sf::Texture texture_;
	sf::Sprite sprite_;
	sf::Font font_;
	sf::SoundBuffer buffer_;
	sf::Sound eplosion_sound_;
	sf::Image icon_;
	sf::Text display_flags_;
	sf::RenderWindow wnd_;

private:
	void initBoard(); // Create board
	void countMines(); // Count mines around cell
	void loadAllAssets(); // Load font, sprites, texture, etc.
	void openEmptyCells(int, int); // Open Empty Cell around Cell
	void displayCells(); // Draw all cells
	void showAllBombs(); // After deth show bombs positions
	 
public:
	Game() = default;
	void Start(); // Start the game
};

#endif
