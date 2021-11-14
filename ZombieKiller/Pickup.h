#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;
class Pickup
{
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	//sprite for pickup
	Sprite m_Sprite;

	//arena item exists in
	IntRect m_Arena;

	//how much pickup worth
	int m_Value;

	//type of pickup 
	// 1 = health, 2 = ammo
	int m_Type;

	//spawning/despawning vars
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDespawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

protected:

public:
	
	
	Pickup(int type);

	void setArena(IntRect arena);

	void Spawn();

	FloatRect getPosition();

	Sprite getSprite();

	void Update(float elaspedTime);

	bool isSpawned();

	int GotIt();

	void Upgrade();

};