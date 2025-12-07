#include <iostream>
#include <Windows.h>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#define MAX_BOARD_SIZE 15
#define BOMB_COUNT 35

sf::RenderWindow* wnd;
sf::Font font;
sf::Text* flags_cnt_text;
sf::Texture texture;
sf::Sprite* sprite;
sf::SoundBuffer* buffer_exp_snd;
sf::SoundBuffer* buffer_win_snd;
sf::Sound* explosion_sound;
sf::Sound* win_sound;
sf::Image icon;

bool is_alive, win;
int cell_width = 32, flags, guessed_positions, bombs;

int hided_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

const int offsets[8][2] = {
	{-1, -1}, {-1, 0}, {-1, 1},
	{0, -1},  {0, 1},  {1, -1},
	{1, 0},   {1, 1}
};

bool isValidCell(int x, int y) {
	return x >= 0 && x < MAX_BOARD_SIZE && y >= 0 && y < MAX_BOARD_SIZE;
}

// Count mines around cell
void countMines() {
	for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
		for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
			if (hided_board[i][j] == 9) continue;

			int bombs_count = 0;
			for (const auto& offset : offsets) {
				int neighbor_x = i + offset[0];
				int neighbor_y = j + offset[1];
				if (isValidCell(neighbor_x, neighbor_y) && hided_board[neighbor_x][neighbor_y] == 9) {
					bombs_count++;
				}
			}
			hided_board[i][j] = bombs_count;
		}
	}
}

// Create board
void initBoard() {
	srand(time(NULL)); is_alive = true; win = false; flags = 0; guessed_positions = 0; bombs = 0;
	for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
		for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
			game_board[i][j] = 10;
			if (rand() % 5 == 0 && bombs < BOMB_COUNT) {
				hided_board[i][j] = 9;
				bombs++; flags = bombs;
			} else hided_board[i][j] = 0;
		}
	}
	countMines();
}

// Load all images, sounds and fonts
void loadAllAssets() {
	// Load main assets
	if (texture.loadFromFile("../../../resources/themes/default.jpg"))
		sprite = new sf::Sprite(texture);
	
	// Set icon to window
	icon.loadFromFile("../../../resources/main_icon.png");

	// Load eplosion sound
	buffer_exp_snd = new sf::SoundBuffer("../../../resources/sounds/explosion.mp3");
	explosion_sound = new sf::Sound(*buffer_exp_snd);
	explosion_sound->setVolume(25);
	
	font.openFromFile("../../../resources/fonts/ArialRegular.ttf");
	flags_cnt_text = new sf::Text(font, std::to_string(35), 22);
	flags_cnt_text->setFillColor(sf::Color::Black);

	// Load win sound
	buffer_win_snd = new sf::SoundBuffer("../../../resources/sounds/win.mp3");
	win_sound = new sf::Sound(*buffer_win_snd);
	initBoard();
}

// Open empty cells around
void openEmptyCells(int x, int y) {
	if (x < 0 || x >= MAX_BOARD_SIZE || y < 0 || y >= MAX_BOARD_SIZE) return;
	if (game_board[x][y] != 10) return;
	game_board[x][y] = hided_board[x][y];
	if (hided_board[x][y] == 0) {
		for (const auto& offset : offsets) 
			openEmptyCells(x + offset[0], y + offset[1]);
	}
}

// Display all cells on window
void displayCells() {
	for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
		for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
			if (game_board[i][j] == 9) {
				game_board[i][j] = hided_board[i][j];
			}

			sprite->setTextureRect(sf::IntRect(
				sf::Vector2i(game_board[i][j] * cell_width, 0),
				sf::Vector2i(game_board[i][j] + 1 * cell_width, cell_width)
			));
			sprite->setScale({ 1.5f, 1.5f });
			sprite->setPosition(sf::Vector2f(i * cell_width * 1.5, j * cell_width * 1.5));
			wnd->draw(*sprite);
		}
	}
}

// After death show bombs positions
void showAllBombs() {
	for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
		for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
			if ((hided_board[i][j] == 9 && game_board[i][j] != 11) ||
				(game_board[i][j] == 11 && hided_board[i][j] != 9)) {
				game_board[i][j] = hided_board[i][j];
			}
		}
	}
}

// Check is all cells open except cells with flag
bool isAllCellOpen() {
	for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
		for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
			if (game_board[i][j] == 11) { continue; }
			if (game_board[i][j] > 9) { return false; }
		}
	}
	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	float size = MAX_BOARD_SIZE * 32 * 1.5;
	loadAllAssets();
	wnd = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(size, size)), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
	wnd->setIcon(icon);
	flags_cnt_text->setPosition(sf::Vector2f(sf::Mouse::getPosition(*wnd)) - sf::Vector2f(-30.f, 30.f));
	while (wnd->isOpen()) {
		sf::Vector2i mouse_position = sf::Mouse::getPosition(*wnd);
		int pos_x = mouse_position.x / (cell_width * 1.5), pos_y = mouse_position.y / (cell_width * 1.5);
		while (const std::optional event = wnd->pollEvent()) {
			if (event->is<sf::Event::Closed>()) { wnd->close(); } 
			else if (const auto* mouse_moved = event->getIf<sf::Event::MouseMoved>()) {
				flags_cnt_text->setPosition(sf::Vector2f(mouse_moved->position) - sf::Vector2f(-30.f, 30.f));
			}
			else if (const auto* button_pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (button_pressed->button == sf::Mouse::Button::Right && is_alive) {
					if (game_board[pos_x][pos_y] == 10 && flags > 0) {
						game_board[pos_x][pos_y] = 11; 
						flags--; 
					}
					if (game_board[pos_x][pos_y] == 11 && hided_board[pos_x][pos_y] == 9) { guessed_positions++; }
				}
				if (button_pressed->button == sf::Mouse::Button::Left && is_alive) {
					if (game_board[pos_x][pos_y] == 10) {
						if (hided_board[pos_x][pos_y] == 0) { openEmptyCells(pos_x, pos_y); }
						if (hided_board[pos_x][pos_y] == 9) {
							explosion_sound->play();
							is_alive = false;
						}
						game_board[pos_x][pos_y] = hided_board[pos_x][pos_y];
					}
					if (game_board[pos_x][pos_y] == 11 && flags < BOMB_COUNT) { 
						game_board[pos_x][pos_y] = 10; 
						flags++; 
					}
				}
				flags = std::clamp(flags, 0, BOMB_COUNT);
				flags_cnt_text->setString(std::to_string(flags));
				if (is_alive && (flags == 0 && guessed_positions == bombs && isAllCellOpen())) {
					win_sound->play();
					win = true;
				}
			} else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::R) initBoard();	// Restart
			}
		}
		if (!is_alive) { showAllBombs(); }
		if (win) { is_alive = false; }
		wnd->clear(sf::Color::White);
		displayCells();
		wnd->draw(*flags_cnt_text);
		wnd->display();
	}
}