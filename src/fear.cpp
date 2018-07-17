#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override something,
//but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"

#include <sapi.h>

using namespace std;


int main()
{
	// Prepare and load game cfg before doing anything else. Config used here not especially important
	unsigned int width = 800;		// Prepare window width
	unsigned int height = 640;		// Prepare window height

	// Create Window
    sf::RenderWindow App( sf::VideoMode( width , height ), "Fear" , sf::Style::Close );  
	Game g( &App );

	if (FAILED(::CoInitialize(NULL)))
		return FALSE;
   

	g.init();					// Initialise game - loads resources

	App.setActive();
	// Start the game loop
    while ( App.isOpen() )
    {	
		g.play(); 											
    }

	::CoUninitialize();
    return EXIT_SUCCESS;
}