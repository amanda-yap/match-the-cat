#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;


int tileSize = 54;
Vector2i offset(46,27);

struct Cat { 
	int x, y, col, row, type, match;
  	Cat() {
		match=0;
	}
} 

grid[10][10];

void swap(Cat c1, Cat c2) {
	std::swap(c1.col, c2.col);
	std::swap(c1.row, c2.row);

	grid[c1.row][c1.col]= c1;
	grid[c2.row][c2.col]= c2;
}

int main()
{
	RenderWindow window( VideoMode( { 740, 480 } ), "Match the Cat" );
	window.setFramerateLimit(60);

	Texture t1, t2;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/cats.png");

	Sprite background(t1), cats(t2);

	// initialise grid and cats
	for (int i = 1; i <=8; i++) {
		for (int j = 1; j <= 8; j++) {
			 grid[i][j].type = rand() % 7;
			 grid[i][j].col = j;
			 grid[i][j].row = i;
			 grid[i][j].x = j * tileSize;
			 grid[i][j].y = i * tileSize;
		}
	}

	int x0, y0, x , y; 
	int click = 0; 
	Vector2i mousePosition;

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<Event::Closed>() ) {
				window.close();
			}

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>()) {
				if (mouseButtonPressed->button == Mouse::Button::Left) {
						click++;
						mousePosition = Mouse::getPosition(window) - offset;
						std::cout << "x: ";
						std::cout << mousePosition.x;
						std::cout << "\n";
						std::cout << "y: ";
						std::cout << mousePosition.y;
						std::cout << "\n";
				}
			}
		}

		// mouse clicks and swap
		if (click == 1) {
			x0 = mousePosition.x / tileSize + 1;
			y0 = mousePosition.y / tileSize + 1;
		}
		if (click == 2) {
			x = mousePosition.x / tileSize + 1;
			y = mousePosition.y / tileSize + 1;
			if (abs(x - x0) + abs(y - y0) == 1) {
				swap(grid[y0][x0], grid[y][x]);
				click = 0;
			}
			else {
				click = 1;
			}
		}

		// moving animation
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				Cat &cat = grid[i][j];
				int dx, dy;
				for (int n=0; n<3; n++) {
					dx = cat.x - cat.col * tileSize;
					dy = cat.y - cat.row * tileSize;
					if (dx) {
						cat.x -= dx/abs(dx);
					}
					if (dy) {
						cat.y -= dy/abs(dy);
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
				cats.setTextureRect(IntRect({cat.type*49, 0}, {49, 49}));
				Vector2f position(cat.x, cat.y);
				cats.setPosition(position);
				Vector2f offsetPosition(offset.x-tileSize, offset.y-tileSize);
        		cats.move(offsetPosition);
				window.draw(cats);
			}
			
		}
		window.display();
	}
}