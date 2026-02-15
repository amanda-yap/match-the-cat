#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;


int tileSize = 54;
Vector2i offset(46,27); // to set (0, 0) as the top left corner of the grid

struct Cat { 
	int x, y, col, row, type, match, alpha;
  	Cat() {
		match = 0;
		alpha = 255;
	}
} 

grid[10][10];

void swap(Cat c1, Cat c2) {
	// swap col and row of cats
	std::swap(c1.col, c2.col);
	std::swap(c1.row, c2.row);

	// set grid position for swapped cats
	grid[c1.row][c1.col]= c1;
	grid[c2.row][c2.col]= c2;
}

int main()
{
	RenderWindow window(VideoMode({ 740, 480 }), "Match the Cat");
	window.setFramerateLimit(150);

	Texture t1, t2;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/cats.png");

	Sprite background(t1), cats(t2);

	// initialise grid with cats
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 8; j++) {
			 grid[i][j].type = rand() % 7; // randomly set cat to one of the seven types
			 grid[i][j].col = j;
			 grid[i][j].row = i;
			 grid[i][j].x = j * tileSize;
			 grid[i][j].y = i * tileSize;
		}
	}

	int x0, y0, x , y; 
	int click = 0; 
	Vector2i mousePosition;
	bool isSwap = false, isMoving = false;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<Event::Closed>()) {
				window.close();
			}

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>()) {
				if (mouseButtonPressed->button == Mouse::Button::Left) {
					if (!isSwap && !isMoving) {
						click++;
						mousePosition = Mouse::getPosition(window) - offset;
					}
					
				}
			}
		}

		// mouse clicks and swap
		if (click == 1) {
			// convert mouse position to row and col
			x0 = mousePosition.x / tileSize + 1;
			y0 = mousePosition.y / tileSize + 1;
			// (x0, y0) is the position of the first cat
		}
		if (click == 2) {
			x = mousePosition.x / tileSize + 1;
			y = mousePosition.y / tileSize + 1;
			// (x, y) is the position of the second cat

			// if the two cats are in the same row or same column, then swap
			if (abs(x - x0) + abs(y - y0) == 1) {
				swap(grid[y0][x0], grid[y][x]);
				isSwap = true;
				click = 0;
			}
			else {
				click = 1;
			}
		}

		// find match
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				int t = grid[i][j].type;
				// vertical match
				if (grid[i - 1][j].type == t &&
					grid[i + 1][j].type == t) {
					grid[i - 1][j].match++;
					grid[i][j].match++;
					grid[i + 1][j].match++;
				}
				// horizontal match
				if (grid[i][j - 1].type == t &&
					grid[i][j + 1].type == t) {
					grid[i][j - 1].match++;
					grid[i][j].match++;
					grid[i][j + 1].match++;
				}
			}
		}
		
		// moving animation
		isMoving = false;
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				Cat &cat = grid[i][j];
				int dx, dy;
				
				// dx and dy are the differences of the previous and new coordinates of the cat
				dx = cat.x - cat.col * tileSize;
				dy = cat.y - cat.row * tileSize;
				if (dx != 0) {
					cat.x -= dx/abs(dx); // move closer to new position until dx = 0 (move by +-1 pixels)
				}
				if (dy != 0) {
					cat.y -= dy/abs(dy);
				}
				if (dx != 0 || dy != 0) {
					isMoving = true;
				}
			}
		}

		// deleting animation
		if (!isMoving) {
			for (int i = 1; i <= 8; i++) {
				for (int j = 1; j <= 8; j++) {
					if (grid[i][j].match && grid[i][j].alpha > 10) {
						grid[i][j].alpha -= 10; 
						isMoving = true;
					}
				}
			}
		}

		// update score
		int score = 0;
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				score += grid[i][j].match;
			}
		}

		// swap back if no match (invalid swap)
		if (isSwap && !isMoving) {
			if (!score) {
				swap(grid[y0][x0], grid[y][x]); 
			}
			isSwap = false;
		}


		// update grid with new cats
		if (!isMoving) {
			for (int i = 8; i > 0; i--) {
				for (int j = 1; j <= 8; j++) {
					if (grid[i][j].match) {
					for(int n = i; n > 0; n--)
						if (!grid[n][j].match) {
							swap(grid[n][j], grid[i][j]); 
							break;
						}
					}
				}
			}
			
			for (int j = 1; j <= 8; j++) {
				for (int i = 8, n = 0; i > 0; i--) {
					if (grid[i][j].match) {
						grid[i][j].type= rand() % 7;
						grid[i][j].y = -tileSize * n++;
						grid[i][j].match = 0;
						grid[i][j].alpha = 255;
					}
				}
			}
		}


		window.clear();
		window.draw(background);

		// draw cats
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				Cat cat = grid[i][j];
				cats.setTextureRect(IntRect({cat.type * 49, 0}, {49, 49})); // set texture to cat type (each cat is a 49x49 square)
				cats.setColor(Color(255, 255, 255, cat.alpha));
				Vector2f position(cat.x, cat.y);
				cats.setPosition(position);
				Vector2f offsetPosition(offset.x-tileSize, offset.y-tileSize);
        		cats.move(offsetPosition); // move cats to be aligned with grid
				window.draw(cats);
			}
			
		}
		window.display();
	}
}