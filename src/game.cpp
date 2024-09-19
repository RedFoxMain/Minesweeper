#include "game.h"
#include <iostream>

// Create board
void Game::InitBoard() {
	srand(time(NULL)); is_alive_ = true; flags_ = 0;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			game_board[i][j] = 10;
			if (rand() % 5 == 0) { hided_board[i][j] = 9; flags_++; }
			else { hided_board[i][j] = 0; }
		}
	}
	CountMines();
}

// Count mines around cell
void Game::CountMines() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			int bombs_count = 0;
			if (hided_board[i][j] == 9) { continue; }
			if (hided_board[i + 1][j] == 9) { bombs_count++; }
			if (hided_board[i][j + 1] == 9) { bombs_count++; }
			if (hided_board[i - 1][j] == 9) { bombs_count++; }
			if (hided_board[i][j - 1] == 9) { bombs_count++; }
			if (hided_board[i + 1][j + 1] == 9) { bombs_count++; }
			if (hided_board[i - 1][j - 1] == 9) { bombs_count++; }
			if (hided_board[i - 1][j + 1] == 9) { bombs_count++; }
			if (hided_board[i + 1][j - 1] == 9) { bombs_count++; }
			hided_board[i][j] = bombs_count;
		}
	}
}

// Start the game
void Game::Start() {
	sf::Texture texture;
	texture.loadFromFile("../../../src/images/tiles.jpg"); // Load assets
	sf::Sprite sprite(texture);
	sf::Font font;
	font.loadFromFile("../../../src/fonts/arial.ttf"); // loading standart font

	InitBoard(); // Init board hide mines and count them
	sf::RenderWindow wnd(sf::VideoMode(cell_width_ * BOARD_SIZE + 200, cell_width_ * BOARD_SIZE), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
	
	sf::Text flags; // Ammount of flags
	flags.setFont(font);
	flags.setCharacterSize(24);
	flags.setFillColor(sf::Color::White);
	flags.setPosition(cell_width_ * BOARD_SIZE + 80, 0);

	sf::Text status; // Game status text
	status.setFont(font);
	status.setCharacterSize(18);
	status.setFillColor(sf::Color::White);
	status.setPosition(cell_width_ * BOARD_SIZE + 10, 33);

	sf::Text help_text; // Help text
	help_text.setFont(font);
	help_text.setCharacterSize(16);
	help_text.setFillColor(sf::Color::White);
	help_text.setPosition(cell_width_ * BOARD_SIZE + 10, 60);

	while (wnd.isOpen()) {
		if (is_alive_) { status.setString(sf::String(L"Статус: Игра")); }
		else { status.setString(sf::String(L"Статус: Проиграл")); }
		flags.setString(sf::String(std::to_string(flags_)));
		help_text.setString(L"Нажми \"R\" для рестарта");

		sf::Vector2i mouse_position = sf::Mouse::getPosition(wnd);
		int pos_x = mouse_position.x / cell_width_, pos_y = mouse_position.y / cell_width_;
		sf::Event event;
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { wnd.close(); }
			if (event.type == sf::Event::MouseButtonPressed) {
				if (is_alive_ && event.key.code == sf::Mouse::Right) { if (game_board[pos_x][pos_y] == 10) { game_board[pos_x][pos_y] = 11; flags_--; } }
				if (is_alive_ && event.key.code == sf::Mouse::Left) {
					if (game_board[pos_x][pos_y] == 10) {
						game_board[pos_x][pos_y] = hided_board[pos_x][pos_y];
						if (game_board[pos_x][pos_y] == 9) { is_alive_ = false; }
					} 
					if (game_board[pos_x][pos_y] == 11) { game_board[pos_x][pos_y] = 10; flags_++; }
				}
			}
			if (event.type == sf::Event::KeyPressed) { if (event.key.code == sf::Keyboard::R) { InitBoard(); } }
		}
		wnd.clear();
		for (int i = 0; i < BOARD_SIZE; ++i) {
			for (int j = 0; j < BOARD_SIZE; ++j) {
				if (game_board[i][j] == 9) { game_board[i][j] = hided_board[i][j]; }
				sprite.setTextureRect(sf::IntRect(game_board[i][j]*cell_width_, 0 , cell_width_, cell_width_));
				sprite.setPosition(i*cell_width_, j*cell_width_);
				wnd.draw(sprite);
			}
		}
		wnd.draw(status);
		wnd.draw(help_text);
		wnd.draw(flags);
		wnd.display();
	}
}