#include "game.h"



Game::Game( sf::RenderWindow* app ) : win( app ) //initialise window
{
	pVoice = NULL;
}

Game::Game() //constructor
{
	
}

void Game::init()
{
	gameFont.loadFromFile("resource/consola.ttf");
	// Setup variables
	oldPosX = 0;
	posX = 0;
	heart_timer = 0;
	distance = 999;
	state = "play";
	// Setup drawing score
	i_score = 0;
	t_score.setCharacterSize(36);
	t_score.setPosition((float)win->getSize().x / 4, (float)win->getSize().y / 2);
	t_dist.setCharacterSize( 36 );
	t_score.setPosition((float)win->getSize().x / 4, (float)win->getSize().y / 4);
	t_dist.setFont(gameFont);
	t_score.setFont(gameFont);
	
	// Set up audio
	heartbeat.openFromFile("resource/heartbeat.ogg");
	enemy.openFromFile("resource/enemy.ogg");
	sf::Listener::setPosition( 0 , 0 , 5 );
	sf::Listener::setDirection( 1 , 0 , 0 );
	heartbeat.setPosition( 0 , 1 , -4 );
	enemy.setAttenuation(10.f);
	enemy.setMinDistance(45.f);
	enemy.setPosition(0, 100, -5);
	enemy.setLoop(true);
	enemy.play();


	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	pVoice->Speak(L"Prepare to play", 0, NULL);
}

void Game::draw()
{
	win->clear(sf::Color( 0 , 0 , 0 ) );
	win->draw(t_score);	// Draw Score
	win->draw(t_dist);	// Draw Distance
}

void Game::getInput()
{
	sf::Event Event;
	while (win->pollEvent(Event))
	{		
		// Close window : exit
		if( Event.type == sf::Event::Closed )
		{
			win->close();
		}
		if( Event.type == sf::Event::MouseMoved )
		{
			posX = sf::Mouse::getPosition().x;
		}
		if (Event.type = sf::Event::MouseButtonPressed)
		{
			// Escape key : exit
			if (Event.mouseButton.button == sf::Mouse::Button::Right)
			{
				win->close();
			}
			if (Event.mouseButton.button == sf::Mouse::Button::Left && (state == "end"))
			{
				enemy.setPosition(0, 100, enemy.getPosition().z);
				state = "play";
			}
		}
	 }
}
	
void Game::update()
{	
	//use the clock and elapsed time to decide what to do	
	float elapsedTime = Clock.getElapsedTime().asMilliseconds()/1000.f;
	if( elapsedTime > 1.0F / FRAMERATE )
	{	
		if( state == "play" )
		{
			//***********************************************************************************************************
			// Within bound checks
			//***********************************************************************************************************
			// Setting enemy sound position
			if( enemy.getPosition().y < -100 ) 
			{
				enemy.setPosition( rand() % 20 - 20.f , 100.f , enemy.getPosition().z );
			}

			// Too far out of bounds to the sides
			if( enemy.getPosition().x > 60 ) enemy.setPosition( 60 , enemy.getPosition().y , enemy.getPosition().z );
			if( enemy.getPosition().x < -60 ) enemy.setPosition( -60 , enemy.getPosition().y , enemy.getPosition().z );


			//***********************************************************************************************************
			//Calculations
			//***********************************************************************************************************
			// Distance calculation
			float dx = enemy.getPosition().x;			// A
			float dy = enemy.getPosition().y;			// B
			distance = sqrt( (dx*dx) + (dy*dy) );		// C2 = Rt|A2 + B2|


			//***********************************************************************************************************
			// Objects updates
			//***********************************************************************************************************
			// Set positions from sounds
			if(posX != oldPosX )
			{
				if(posX > oldPosX) enemy.setPosition( enemy.getPosition().x - 2 , enemy.getPosition().y , enemy.getPosition().z );
				if(posX < oldPosX) enemy.setPosition( enemy.getPosition().x + 2 , enemy.getPosition().y , enemy.getPosition().z );
			}

			enemy.setPosition( enemy.getPosition().x , enemy.getPosition().y - 1 , enemy.getPosition().z );

			oldPosX = posX;

			//***********************************************************************************************************
			// Calls to play sounds
			//***********************************************************************************************************
			beat(); // heartbeat
			fscore(); // score drawer

			//***********************************************************************************************************
			// Any debug stuff to cout
			//***********************************************************************************************************
			if( distance < 15 )
			{
				if (SUCCEEDED(hr))
				{
					std::string s = "Game over. Your final score is " + std::to_string(i_score) + ". Left click to play again, or right click to exit";
					int len;
					int slength = (int)s.length() + 1;
					len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
					wchar_t* buf = new wchar_t[len];
					MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
					std::wstring r(buf);
					delete[] buf;
					std::wstring stemp = r;
					LPCWSTR result = stemp.c_str();

					pVoice->Speak(result, 0, NULL);
					//pVoice->Release();
					//pVoice = NULL;
				}
				state = "end";
			}
		}
		


		Clock.restart();
	}
}

void Game::play()
{
	//the game loop
	draw();											//the offscreen buffer
	getInput();										//from the user to check for exit
	update();										//integrate user input and any game movement
	win->display();									//display the rendered frame
}

Game::~Game()
{
	//delete anything on heap in case of creation
	pVoice->Release();
	pVoice = NULL;
}

void Game::beat(){
	if( distance >= 100 )
			{
				if(heart_timer >= FRAMERATE )
				{
					heartbeat.stop();
					heartbeat.play();
					heart_timer = 0;
				} else {
					++heart_timer;
				}
			}
			if( distance >= 80 && distance < 100)
			{
				if(heart_timer >= FRAMERATE*0.8 )
				{
					heartbeat.stop();
					heartbeat.play();
					heart_timer = 0;
				} else {
					++heart_timer;
				}
			}
			if( distance >= 60 && distance < 80)
			{
				if(heart_timer >= FRAMERATE*0.6 )
				{
					heartbeat.stop();
					heartbeat.play();
					heart_timer = 0;
				} else {
					++heart_timer;
				}
			}
			if( distance >= 40 && distance < 60)
			{
				if(heart_timer >= FRAMERATE*0.45 )
				{
					heartbeat.stop();
					heartbeat.play();
					heart_timer = 0;
				} else {
					++heart_timer;
				}
			}
			if( distance >= 20 && distance < 40)
			{
				if(heart_timer >= FRAMERATE*0.4 )
				{
					heartbeat.stop();
					heartbeat.play();
					heart_timer = 0;
				} else {
					++heart_timer;
				}
			}
			if( distance >= 15 && distance < 20)
			{
				if(heart_timer >= FRAMERATE*0.35 )
				{
					heartbeat.stop();
					heartbeat.play();
					heart_timer = 0;
				} else {
					++heart_timer;
				}
			}
}

void Game::fscore()
{
	// Score handler & updater
	++i_score; // add 1 to score each frame
	oss << i_score;
	string d = oss.str();
	oss.str(" ");
	s_score = "Score: " + d;	
	t_score.setString(s_score);
	oss << distance;
	string e = oss.str();
	oss.str(" ");
	s_dist = "Distance: " + e ;	
	t_dist.setString(s_dist);
}