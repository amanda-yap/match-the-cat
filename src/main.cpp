#include <SFML/Graphics.hpp>
using namespace sf;

int main()
{
	RenderWindow window( VideoMode( { 740, 480 } ), "Match The Cat" );
	window.setFramerateLimit(60);

	CircleShape shape( 100.f );
	shape.setFillColor( Color::Green );

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<Event::Closed>() )
				window.close();
		}

		window.clear();
		window.draw( shape );
		window.display();
	}
}
