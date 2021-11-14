#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Zombie
{
private:
	//zombie speeds
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//zombie hp
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//give speed a range so zombiess dont all move at same speed
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	//zombie location
	Vector2f m_Postion;

	Sprite m_Sprite;

	//speed of this specific zombie
	float m_Speed;

	float m_Health;

	bool m_Alive = true;
protected:

public:
	Zombie();

	bool Hit();

	bool IsAlive();

	void Spawn(float startX, float startY, int type, int seed);

	FloatRect getPosition();

	Sprite getSprite();

	void Update(float elaspedTime, Vector2f playerLocation);
};