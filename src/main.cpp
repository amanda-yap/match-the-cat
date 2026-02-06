#include <SFML/Graphics.hpp>
using namespace sf;

#include <filesystem>
#include <iostream>

int tileSize = 54;
Vector2i offset(46,27);

struct Cat { 
	int x, y, col, row, type, match;
  	Cat() {
		match=0;
	}
} 

grid[10][10];

int main()
{
	RenderWindow window( VideoMode( { 740, 480 } ), "Match the Cat" );
	window.setFramerateLimit(60);

	Texture t1, t2;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/cats.png");

	Sprite background(t1), cats(t2);

	/* Initialise grid and cats */
	for (int i = 1; i <=8; i++) {
		for (int j = 1; j <= 8; j++) {
			 grid[i][j].type = rand() % 7;
			 grid[i][j].col = j;
			 grid[i][j].row = i;
			 grid[i][j].x = j * tileSize;
			 grid[i][j].y = i * tileSize;
		}
	}


	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<Event::Closed>() ) {
				window.close();
			}
		}

		window.clear();
		window.draw(background);

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

		window.draw(cats);
		window.display();
	}
}