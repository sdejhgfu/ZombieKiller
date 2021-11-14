// SetupSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ZombieKillerh.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;
int main()
{
	TextureHolder holder;
	//keep track of game state
	enum class GameState
	{
		PAUSED, LEVELING_UP,GAMEOVER,PLAYING
	};
	GameState gState = GameState::GAMEOVER;

	//get screen resolution/create window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),"Zombie Killer", Style::Default);

	//create view object
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	//clock/game time
	Clock clock;

	//time player has been in playing state
	Time gameTimeTotal;

	//mouse world and screen locations
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	IntRect arena;

	Player player;

	//create background
	VertexArray background;

	//load texture
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	//zombie set up
	int numZombies = 0;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//bullet set up
	Bullet bullets[100];
	int currentBullet = 0;
	int spareBullets = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastBulletFired;

	//Pickup setup
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//replace mouse with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrossHair;
	Texture textureCrossHair = TextureHolder::GetTexture("graphics/crosshair.png");

	spriteCrossHair.setTexture(textureCrossHair);
	spriteCrossHair.setOrigin(25, 25);

	//score set up
	int score = 0;
	int highScore = 0;

	//Game Over sprite
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);


	//****************
	//HUD SETUP
	//****************

	//crete hud view
	View hudView(sf::FloatRect(0,0,resolution.x,resolution.y));
	
	//create ammo sprite
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(resolution.x*.01f, resolution.y*.845f);

	//load font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//paused set up
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(85);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(resolution.x*.4f, resolution.y*.4f);
	pausedText.setString("Press Enter \n to continue");

	//Game Over
	Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setCharacterSize(80);
	gameoverText.setFillColor(Color::White);
	gameoverText.setPosition(resolution.x*.33f, resolution.y*.4f);
	gameoverText.setString("Press Enter to play");

	//Level up
	Text levelupText;
	levelupText.setFont(font);
	levelupText.setFillColor(Color::White);
	levelupText.setCharacterSize(60);
	levelupText.setPosition(resolution.x*.21f, resolution.y*.32f);
	std::stringstream levelupStream;
	levelupStream << "1 - Increase rate of fire" << 
		"\n2 - Increase clip size, takes effect next reload" << 
		"\n3 - Increase max health" << 
		"\n4 - Increase run speed" <<
		"\n5 - Better/more frequent health drops" << 
		"\n6 - Better/more frequent ammo drops";
	levelupText.setString(levelupStream.str());

	//Ammo Text
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setFillColor(Color::White);
	ammoText.setCharacterSize(50);
	ammoText.setPosition(resolution.x*.04f, resolution.y*.85f);

	//Score text
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setFillColor(Color::White);
	scoreText.setCharacterSize(50);
	scoreText.setPosition(resolution.x*.01f, 0);

	//check to see for a previous highscore
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> highScore;
		inputFile.close();
	}

	//Highscore text
	Text highscoreText;
	highscoreText.setFont(font);
	highscoreText.setFillColor(Color::White);
	highscoreText.setCharacterSize(50);
	highscoreText.setPosition(resolution.x*.8f, 0);
	std::stringstream s;
	s << "High Score:" << highScore;
	highscoreText.setString(s.str());

	//Remain Zombie text
	Text zombiesRemainingText;
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(50);
	zombiesRemainingText.setPosition(resolution.x*.7f, resolution.y*.85f);	
	zombiesRemainingText.setString("Zombies: 100");

	//Wave number
	int wave =0;
	Text waveNumText;
	waveNumText.setFillColor(Color::White);
	waveNumText.setCharacterSize(50);
	waveNumText.setFont(font);
	waveNumText.setPosition(resolution.x*.2f, resolution.y*.85f);
	waveNumText.setString("Wave: 0");

	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(resolution.x*.4f, resolution.y*.85f);

	//last hud update
	int framesSinceLastHUDUpdate = 0;

	//how often in frames should HUD be updated
	int fpsMeasurementFrameInterval = 1000;


	//****************
	// AUDIO SETUP
	//****************

	//hit sfx
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	//blood splat sfx
	SoundBuffer bloodSplatBuffer;
	bloodSplatBuffer.loadFromFile("sound/splat.wav");
	Sound bloodSplat;
	bloodSplat.setBuffer(bloodSplatBuffer);

	//shoot sfx
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	//reload fail sfx
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	//reload succeed sfx
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	//powerup sfx
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	//pickup sfx
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/powerup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	//game loop
	while (window.isOpen())
	{

		
		/*
		****************************
		Handle Input
		****************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();

			

			if (event.type == Event::KeyPressed)
			{
				//pausing
				if (event.key.code == Keyboard::Return && gState == GameState::PLAYING)
				{
					gState = GameState::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && gState == GameState::PAUSED)
				{
					gState = GameState::PLAYING;
					clock.restart();
				}

				//starting a new game
				else if (event.key.code == Keyboard::Return && gState == GameState::GAMEOVER)
				{
					gState = GameState::LEVELING_UP;

					wave = 0;
					score = 0;

					currentBullet = 0;
					spareBullets = 24;
					bulletsInClip = 6;
					fireRate = 1;

					player.ResetPlayerStats();
				}
				if (gState == GameState::PLAYING)
				{
					if (event.key.code == Keyboard::R)
					{
						//reloading
						if (bulletsInClip == clipSize)
						{
							reloadFailed.play();
						}
						else if (spareBullets >= clipSize)
						{
							bulletsInClip = clipSize;
							spareBullets -= clipSize;
							reload.play();
						}
						else if (spareBullets > 0)
						{
							bulletsInClip = spareBullets;
							spareBullets = 0;
							reload.play();
						}
						else
						{
							reloadFailed.play();
						}
					}
				}				
			}
		}//end polling event

		//player quit
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (gState == GameState::PLAYING)
		{
			//movement inputs
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.MoveUp();
			}
			else
			{
				player.StopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.MoveDown();
			}
			else
			{
				player.StopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.MoveLeft();
			}
			else
			{
				player.StopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.MoveRight();
			}
			else
			{
				player.StopRight();
			}
			//firing input
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastBulletFired.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					//fire shot using player as starting point and mouse location as end point
					bullets[currentBullet].Shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastBulletFired = gameTimeTotal;
					bulletsInClip--;

					shoot.play();
				}
			}//end fire
		}

		if (gState == GameState::LEVELING_UP)
		{
			if (event.key.code == Keyboard::Num1)
			{
				//increase firerate
				fireRate++;
				gState = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num2)
			{
				//increase clip
				clipSize += clipSize;
				gState = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num3)
			{
				//increase health
				player.UpgradeHealth();
				gState = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num4)
			{
				//increase speed
				player.UpgradeSpeed();
				gState = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num5)
			{
				//upgrade health pickup
				healthPickup.Upgrade();
				gState = GameState::PLAYING;
			}
			if (event.key.code == Keyboard::Num6)
			{
				//upgade ammo pickup
				ammoPickup.Upgrade();
				gState = GameState::PLAYING;
			}
			if (gState == GameState::PLAYING)
			{
				//increase wave counter
				wave++;

				//prepare level
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;
					

				int tileSize = CreateBackgroundArena(background,arena);
				
				//spawn player in middle of arena
				player.Spawn(arena, resolution, tileSize);

				//configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//create horde
				numZombies = 10 * wave;

				//clear memory cache
				delete[] zombies;
				zombies = CreateHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//play powerup sound
				powerup.play();

				//reset clock to prevent frame jumping
				clock.restart();
			}
		}//end level up

		/*
		*****************
		UPDATE FRAME
		****************
		*/
		if (gState == GameState::PLAYING)
		{
			//std::cout << "Game Being Played ";


			//update dt
			Time deltaTime = clock.restart();

			//Update total time
			gameTimeTotal += deltaTime;


			float dtAsSeconds = deltaTime.asSeconds();
			
			//get mouse location
			mouseScreenPosition = Mouse::getPosition();

			//get mouse world location
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(),mainView);

			//update crosshair location
			spriteCrossHair.setPosition(mouseWorldPosition.x, mouseWorldPosition.y);

			//update player
			player.Update(dtAsSeconds, Mouse::getPosition());
				
			//make a note of new player position
			Vector2f playerPosition(player.getCenter());

			//make view center around player
			mainView.setCenter(player.getCenter());


			//loop through	zombies and update
			if (numZombiesAlive > 0)
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].IsAlive())
				zombies[i].Update(deltaTime.asSeconds(), playerPosition);
			}

			//loop through bullets and update
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].getInFlight())
				{
					bullets[i].Update(deltaTime.asSeconds());
				}
			}

			//update pickups
			healthPickup.Update(dtAsSeconds);
			ammoPickup.Update(dtAsSeconds);

			//COLLISION CHECKS
			//zombies shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].getInFlight() && zombies[j].IsAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							bullets[i].Stop();

							if (zombies[j].Hit())
							{
								score += 10;
								if (score >= highScore)
								{
									highScore = score;
								}
								numZombiesAlive--;

								//check to see if all zombies are dead
								if (numZombiesAlive <= 0)
								{
									gState = GameState::LEVELING_UP;
								}
							}
							bloodSplat.play();
						}
					}
				}
			}//end of zombies shot
			
			//Player hit?
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].IsAlive())
				{
					if (player.Hit(gameTimeTotal))
					{
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						gState = GameState::GAMEOVER;
						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << highScore;
						outputFile.close();
					}
				}
			}//end player hit

			//play hit pickup?
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.IncreaseHealthLevel(healthPickup.GotIt());
				pickup.play();
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				spareBullets += ammoPickup.GotIt();
				reload.play();
			}

			//std::cout << "Player Health: " << player.getHealth();
			
			//size up the healthbar
			healthBar.setSize(Vector2f(player.getHealth()*300, 70));
					
			//incremeant the number of frames
			//since the last hud calculation
			framesSinceLastHUDUpdate++;

			//check if hud should be updated
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				//update hud text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHighscore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;


				//ammo text
				ssAmmo << bulletsInClip << "/" << spareBullets;
				ammoText.setString(ssAmmo.str());

				//score text
				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());

				//highscore text
				ssHighscore << "Hiscore: " << highScore;
				highscoreText.setString(ssHighscore.str());

				//wave text
				ssWave << "Wave: " << wave;
				waveNumText.setString(ssWave.str());

				//alive zombie text
				ssZombiesAlive << "Zombies left: " <<numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				//reset frame counter
				framesSinceLastHUDUpdate = 0;

			}//end HUD update

		}
		/*
		***********
		Draw Scene
		***********
		*/
		if (gState == GameState::PLAYING)
		{
			window.clear();
			window.setMouseCursorVisible(false);

			//set mainview to show in window
			//draw the stuff around it
			window.setView(mainView);

			//draw background
			window.draw(background, &textureBackground);

			//draw zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}
			//draw bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].getInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			//draw player
			window.draw(player.getSprite());

			//draw pickups if they are spawned
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			//draw crosshair
			window.draw(spriteCrossHair);

			//switch to hud view
			window.setView(hudView);

			//draw all hud elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(highscoreText);
			window.draw(healthBar);
			window.draw(waveNumText);
			window.draw(zombiesRemainingText);

		}
		if (gState == GameState::PAUSED)
		{
			window.setMouseCursorVisible(true);
			window.draw(pausedText);
		}
		if (gState == GameState::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelupText);
		}
		if (gState == GameState::GAMEOVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameoverText);
			window.draw(scoreText);
			window.draw(highscoreText);
		}
		window.display();

	}//end game

	//clean up
	delete[] zombies;

	
	return 0;
}
