#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

constexpr int GRID_SIZE = 8;
int tileSize = 54;
Vector2i offset(46,27); // to set (0, 0) as the top left corner of the grid

struct Cat { 
	int x, y, type, match, alpha;
  	Cat() {
		match = 0;
		alpha = 255;
	}
} 

grid[GRID_SIZE][GRID_SIZE];

int main()
{
	RenderWindow window(VideoMode({ 740, 480 }), "Match the Cat");
	window.setFramerateLimit(150);

	Texture t1, t2;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/cats.png");

	Sprite background(t1), cats(t2);

	// initialise grid with cats
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			 grid[i][j].type = rand() % 7; // randomly set cat to one of the seven types
			 grid[i][j].x = (j + 1) * tileSize;
			 grid[i][j].y = (i + 1) * tileSize;
		}
	}

	int x0, y0, x , y; 
	int click = 0; 
	Vector2i mousePosition;
	bool isSwapping = false, isMoving = false;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<Event::Closed>()) {
				window.close();
			}

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>()) {
				if (mouseButtonPressed->button == Mouse::Button::Left) {
					if (!isSwapping && !isMoving) {
						click++;
						mousePosition = Mouse::getPosition(window) - offset;
					}
				}
			}
		}

		// convert mouse position to row and col
		int mx = mousePosition.x / tileSize;
		int my = mousePosition.y / tileSize;

		// if a tile has been clicked that is not within the grid, reset click count
		if (mx < 0 || mx >= 8 || my < 0 || my >= 8) {
			click = 0;
		}

		// mouse clicks and swap
		if (click == 1) {
			x0 = mx;
			y0 = my;
			// (x0, y0) is the position of the first cat
		}
		if (click == 2) {
			x = mx;
			y = my;
			// (x, y) is the position of the second cat
		
			// if the two cats are in the same row or same column and one tile apart, then swap
			if ((abs(x - x0) == 1 && y == y0) ||
				(abs(y - y0) == 1 && x == x0)) {
				std::swap(grid[y0][x0], grid[y][x]);
				isSwapping = true;
				click = 0;
			}
			else {
				click = 1;
			}
		}

		// row match finding
		for (int i = 0; i < GRID_SIZE; i++) {
			int count = 1;
			for (int j = 1; j < GRID_SIZE; j++) {
				if (grid[i][j].type == grid[i][j - 1].type) {
					count++;
				}
				else {
					if (count >= 3) {
						for (int k = 0; k < count; k++) {
							grid[i][j - 1 - k].match++;
						}
					}
					count = 1;
				}
			}
			// add matches for end of row
			if (count >= 3) {
				for (int k = 0; k < count; k++)
					grid[i][GRID_SIZE - 1 - k].match++;
			}
		}


		// column match finding
		for (int j = 0; j < GRID_SIZE; j++) {
			int count = 1;
			for (int i = 1; i < GRID_SIZE; i++) {
				if (grid[i][j].type == grid[i - 1][j].type) {
					count++;
				} 
				else {
					if (count >= 3) {
						for (int k = 0; k < count; k++) {
							grid[i - 1 - k][j].match++;
						}
					}
					count = 1;
				}
			}
			// add matches for end of column
			if (count >= 3) {
				for (int k = 0; k < count; k++)
					grid[GRID_SIZE - 1 - k][j].match++;
			}
		}
		
		isMoving = false;

		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				Cat &cat = grid[i][j];
				int targetX = (j + 1) * tileSize;
				int targetY = (i + 1) * tileSize;
				int dx = cat.x - targetX;
				int dy = cat.y - targetY;

				cat.x -= (dx > 0) - (dx < 0);
				cat.y -= (dy > 0) - (dy < 0);

				if (dx != 0 || dy != 0) {
					isMoving = true;
				}
			}
		}

		int score = 0;

		if (!isMoving) {
			for (int i = 0; i < GRID_SIZE; i++) {
				for (int j = 0; j < GRID_SIZE; j++) {
					Cat &cat = grid[i][j];
					if (cat.match) {
						score += cat.match;
						if (cat.alpha > 10) {
							cat.alpha -= 10;
							isMoving = true;
						}
					}
				}
			}
		}


		// swap back if no match (invalid swap)
		if (isSwapping && !isMoving) {
			if (score == 0) {
				std::swap(grid[y0][x0], grid[y][x]); 
			}
			isSwapping = false;
		}


		if (!isMoving) {
			// gravity
			for (int j = 0; j < GRID_SIZE; j++) {
				int nextFreeRow = GRID_SIZE - 1;  // bottom-most free row
				for (int i = GRID_SIZE - 1; i >= 0; i--) {
					if (!grid[i][j].match) { // cat falls if no match
						if (i != nextFreeRow)
							std::swap(grid[i][j], grid[nextFreeRow][j]);
						nextFreeRow--; // move next free row up
					}
				}
			}
				
			// make new cats
			for (int j = 0; j < GRID_SIZE; j++) {
				int spawnCount = 0;
				for (int i = GRID_SIZE - 1; i >= 0; i--) {
					Cat &cat = grid[i][j];
					if (cat.match) {
						cat.type = rand() % 7;
						cat.y = -tileSize * (spawnCount++);
						cat.match = 0;
						cat.alpha = 255;
					}
				}
			}
		}


		window.clear();
		window.draw(background);

		// draw cats
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				Cat &cat = grid[i][j];
				cats.setTextureRect(IntRect({cat.type * 49, 0}, {49, 49}));
				cats.setColor(Color(255, 255, 255, cat.alpha));
				cats.setPosition(Vector2f(cat.x, cat.y));
				cats.move(Vector2f(offset.x - tileSize, offset.y - tileSize)); // move cats to be aligned with grid
				window.draw(cats);
			}
		}
		window.display();
	}
}