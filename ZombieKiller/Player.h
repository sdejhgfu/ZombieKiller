#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Player 
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	//player location
	Vector2f m_Position;
		
	//player sprite
	Sprite m_Sprite;

	//player texture
	Texture m_Texture;

	//screen resolution
	Vector2f m_Resolution;

	//arena size
	IntRect m_Arena;

	//how big each tile is
	int m_TileSize;

	//Checking Direction
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_RightPressed;
	bool m_LeftPressed;

	//player current health
	float m_Health;

	//player max health
	float m_MaxHealth;

	//stat increase amount by
	float m_InceasePercent;

	//last time player hit
	Time m_LastHit;

	//player movespeed
	float m_Speed;


public:
	Player();

	void Spawn(IntRect arena, Vector2f resolution, int tilesize);

	void ResetPlayerStats();

	bool Hit(Time timeHit);

	Time getLastHit();

	FloatRect getPosition();

	Vector2f getCenter();

	float getRotation();

	Sprite getSprite();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void StopLeft();
	void StopRight();
	void StopUp();
	void StopDown();

	void Update(float elaspedTime, Vector2i mousePosition);

	void UpgradeSpeed();

	void UpgradeHealth();

	void IncreaseHealthLevel(int amount);

	float getHealth();
};