#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <cmath>


#include <sapi.h>

using namespace std;

const int FRAMERATE=10;				// the number of updates per second

class Game
{
public:
	Game( sf::RenderWindow* );
	Game();							// default constructor - good practice to always include
	
	void init();					// access resource
	void draw();					// draws the game frame
	void getInput();				// gets the input from user
	void update();					// takes care of any updating
	void play();

	~Game();						// destructor

	void beat();
	void fscore();

protected:
	//required for all
	sf::RenderWindow* win;			// a pointer to the window to draw to
	sf::Clock Clock;				// for timing the 

	ostringstream oss;

	

private:
	sf::String s_score , s_dist;
	sf::Text t_score, t_dist;
	sf::Music heartbeat , enemy;
	int posX, oldPosX;

	int i_score;
	float distance , heart_timer;
	string state;


	sf::Font gameFont;

	ISpVoice * pVoice;
	HRESULT hr;
};
#endif