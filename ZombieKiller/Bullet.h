#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet
{
private:
	
	Vector2f m_Position;

	RectangleShape m_BulletShape;

	bool m_InFlight = false;

	float m_BulletSpeed = 1000;


	//bullet distance travelled per frame, derived from m_BulletSpeed
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//bullet boundries
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

protected:

public:
	Bullet();

	void Stop();

	bool getInFlight();

	void Shoot(float startX, float startY, float targetX, float targetY);

	FloatRect getPosition();

	RectangleShape getShape();

	void Update(float elaspedTime);
};