#include "game.h"

// Create board
void Game::initBoard() {
	srand(time(NULL)); is_alive_ = true; win_ = false; flags_ = 0; guessed_positions_ = 0; bombs_ = 0;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			game_board[i][j] = 10;
			if (rand() % 5 == 0) { hided_board[i][j] = 9; bombs_++; flags_ = bombs_; }
			else { hided_board[i][j] = 0; }
		}
	}
	countMines();
}

// Count mines around cell
void Game::countMines() {
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

// Load all images, sounds and fonts
void Game::loadAllAssets() {
	// Load main assets
	texture_.loadFromFile("../../../src/images/tiles.jpg"); 
	sprite_.setTexture(texture_);

	// Set icon to window
	icon_.loadFromFile("../../../src/images/main_icon.png"); // Set icon to window
	wnd_.setIcon(icon_.getSize().x, icon_.getSize().y, icon_.getPixelsPtr());

	// Load Font
	font_.loadFromFile("../../../src/fonts/arial.ttf");

	// Load eplosion sound
	buffer_.loadFromFile("../../../src/sounds/explosion.mp3");
	eplosion_sound_.setBuffer(buffer_);

	// Set flags count
	display_flags_.setFont(font_);
	display_flags_.setCharacterSize(32);
	display_flags_.setFillColor(sf::Color::Black);
	display_flags_.setPosition(140, 10);
}

// Open empty cells around
void Game::openEmptyCells(int x, int y) {}

// Display all cells on window
void Game::displayCells() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if (game_board[i][j] == 9) { game_board[i][j] = hided_board[i][j]; }
			sprite_.setTextureRect(sf::IntRect(game_board[i][j] * cell_width_, 0, cell_width_, cell_width_));
			sprite_.setPosition(i * cell_width_, (j + 2) * cell_width_);
			wnd_.draw(sprite_);
		}
	}
}

void Game::showAllBombs() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if (hided_board[i][j] == 9) { game_board[i][j] = hided_board[i][j]; }
		}
	}
}

// Start the game
void Game::Start() {
	initBoard(); // Init board hide mines and count them
	wnd_.create(sf::VideoMode(cell_width_ * BOARD_SIZE, (cell_width_+6.4) * BOARD_SIZE), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
	loadAllAssets();

	while (wnd_.isOpen()) {
		display_flags_.setString(std::to_string(flags_));
		sf::Vector2i mouse_position = sf::Mouse::getPosition(wnd_);
		int pos_x = mouse_position.x / cell_width_, pos_y = mouse_position.y / cell_width_ - 2;
		sf::Event event;
		if (!is_alive_) { showAllBombs();  }
		while (wnd_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { wnd_.close(); }
			if (event.type == sf::Event::MouseButtonPressed) {
				if ((win_ || is_alive_) && event.key.code == sf::Mouse::Right) { if (game_board[pos_x][pos_y] == 10) { game_board[pos_x][pos_y] = 11; flags_--; } }
				if ((win_ || is_alive_) && event.key.code == sf::Mouse::Left) {
					if (game_board[pos_x][pos_y] == 10) {
						game_board[pos_x][pos_y] = hided_board[pos_x][pos_y];
						if (game_board[pos_x][pos_y] == 11 && hided_board[pos_x][pos_y] == 9) { game_board[pos_x][pos_y] = hided_board[pos_x][pos_y]; guessed_positions_++; }
						if (guessed_positions_ == bombs_) { win_ = true; }
						if (game_board[pos_x][pos_y] == 9) { eplosion_sound_.play(); is_alive_ = false; }
					} 
					if (game_board[pos_x][pos_y] == 11) { game_board[pos_x][pos_y] = 10; flags_++; } 
				}
			}
			if (event.type == sf::Event::KeyPressed) { if (event.key.code == sf::Keyboard::R) { initBoard(); } }
		}
		
		wnd_.clear(sf::Color::White);
		displayCells();
		wnd_.draw(display_flags_);
		wnd_.display();
	}
}