#include <SFML/Graphics.hpp>
using namespace sf;

#include <filesystem>
#include <iostream>


int main()
{
	RenderWindow window( VideoMode( { 740, 480 } ), "Match the Cat" );
	window.setFramerateLimit(60);

	Texture t1, t2;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/cats.png");

	Sprite background(t1), cats(t2);

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<Event::Closed>() )
				window.close();
		}
		window.clear();
		window.draw(background);
		window.draw(cats);
		window.display();
	}
}