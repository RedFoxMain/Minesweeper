#include "game.h"
#include "config.h"

// Create board
void Game::initBoard() {
	srand(time(NULL)); is_alive_ = true; win_ = false; flags_ = 0; guessed_positions_ = 0; bombs_ = 0;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			this->game_board[i][j] = 10;
			if (rand() % 5 == 0 && bombs_ < 10) { this->hided_board[i][j] = 9; bombs_++; flags_ = bombs_; } else { this->hided_board[i][j] = 0; }
		}
	}
	loadAllAssets();
	countMines();
}

// Count mines around cell
void Game::countMines() {
	const int offsets[8][2] = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1},  {0, 1},  {1, -1},
		{1, 0},   {1, 1}
	};

	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if (this->hided_board[i][j] == 9) {
				continue;
			}

			int bombs_count = 0;
			for (const auto& offset : offsets) {
				int neighbor_x = i + offset[0];
				int neighbor_y = j + offset[1];
				if (isValidCell(neighbor_x, neighbor_y) && this->hided_board[neighbor_x][neighbor_y] == 9) {
					bombs_count++;
				}
			}
			this->hided_board[i][j] = bombs_count;
		}
	}
}

// Load all images, sounds and fonts
void Game::loadAllAssets() {
	// Load main assets
	texture_.loadFromFile(LOAD_RESOURCE("images/tiles.jpg")); 
	sprite_.setTexture(texture_);

	// Set icon to window
	icon_.loadFromFile(LOAD_RESOURCE("images/main_icon.png"));
	wnd_.setIcon(icon_.getSize().x, icon_.getSize().y, icon_.getPixelsPtr());

	// Load eplosion sound
	buffer_exp_snd_.loadFromFile(LOAD_RESOURCE("sounds/explosion.mp3"));
	eplosion_sound_.setBuffer(buffer_exp_snd_);

	// Load win sound
	buffer_win_snd_.loadFromFile(LOAD_RESOURCE("sounds/win.mp3"));
	win_sound_.setBuffer(buffer_win_snd_);
}

// Open empty cells around
void Game::openEmptyCells(int x, int y) {
	if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) { return; }
	if (game_board[x][y] != 10) { return; }
	game_board[x][y] = hided_board[x][y];
	if (hided_board[x][y] == 0) {
		openEmptyCells(x - 1, y);
		openEmptyCells(x + 1, y);
		openEmptyCells(x, y - 1);
		openEmptyCells(x, y + 1);
		openEmptyCells(x - 1, y - 1);
		openEmptyCells(x - 1, y + 1);
		openEmptyCells(x + 1, y - 1);
		openEmptyCells(x + 1, y + 1);
	}
}

// Display all cells on window
void Game::displayCells() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if (this->game_board[i][j] == 9) {
				this->game_board[i][j] = this->hided_board[i][j];
			}
			sprite_.setTextureRect(sf::IntRect(this->game_board[i][j] * cell_width_, 0, cell_width_, cell_width_));
			sprite_.setPosition(i * cell_width_, j * cell_width_);
			wnd_.draw(sprite_);
		}
	}
}

// After deth show bombs positions
void Game::showAllBombs() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if ((this->hided_board[i][j] == 9 && this->game_board[i][j] != 11) ||
				(this->game_board[i][j] == 11 && this->hided_board[i][j] != 9)) {
				this->game_board[i][j] = this->hided_board[i][j];
			}
		}
	}
}

// Check is all cells open except cells with flag
bool Game::isAllCellOpen() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if (this->game_board[i][j] == 11) { continue; }
			if (this->game_board[i][j] > 9) { return false; }
		}
	}
	return true;
}

bool Game::isValidCell(int x, int y) {
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// Start the game
void Game::start() {
	wnd_.create(sf::VideoMode(cell_width_ * BOARD_SIZE, cell_width_ * BOARD_SIZE), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
	initBoard(); // Init board hide mines and count them and load all assets
	while (wnd_.isOpen()) {
		sf::Vector2i mouse_position = sf::Mouse::getPosition(wnd_);
		int pos_x = mouse_position.x / cell_width_, pos_y = mouse_position.y / cell_width_;
		sf::Event event;
		while (wnd_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { wnd_.close(); }
			if (event.type == sf::Event::MouseButtonPressed) {
				if (is_alive_ && (event.key.code == sf::Mouse::Right)) {
					if (this->game_board[pos_x][pos_y] == 10) { this->game_board[pos_x][pos_y] = 11; flags_--; }
					if (this->game_board[pos_x][pos_y] == 11 && this->hided_board[pos_x][pos_y] == 9) { guessed_positions_++; }
				}
				if (is_alive_ && (event.key.code == sf::Mouse::Left)) {
					if (game_board[pos_x][pos_y] == 10) {
						if (this->hided_board[pos_x][pos_y] == 0) { openEmptyCells(pos_x, pos_y); }
						if (this->hided_board[pos_x][pos_y] == 9) { eplosion_sound_.play();  is_alive_ = false; }
						this->game_board[pos_x][pos_y] = this->hided_board[pos_x][pos_y];
					}
					if (this->game_board[pos_x][pos_y] == 11) { this->game_board[pos_x][pos_y] = 10; flags_++; }
				}
				if (is_alive_ && (flags_ == 0 && guessed_positions_ == bombs_ && isAllCellOpen())) { win_sound_.play(); win_ = true; }
			}
			if (event.type == sf::Event::KeyPressed) { 
				if (event.key.code == sf::Keyboard::R) { initBoard(); } 
				
			}
		}
		if (!is_alive_) { showAllBombs(); }
		if (win_) { is_alive_ = false; }
		wnd_.clear(sf::Color::White);
		displayCells();
		wnd_.display();
	}
}