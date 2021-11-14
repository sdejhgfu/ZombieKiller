#include "Bullet.h"
#include <SFML/Graphics.hpp>


Bullet::Bullet()
{
	m_BulletShape.setSize(Vector2f(2, 2));
}

void Bullet::Stop()
{
	m_InFlight = false;
}

bool Bullet::getInFlight()
{
	return m_InFlight;
}

void Bullet::Shoot(float startX, float startY, float targetX, float targetY)
{
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	//calc flight path
	float gradient = (startX - targetX) / (startY - targetY);

	//make sure flight path is always positve
	if (gradient < 0)
	{
		gradient *= -1;
	}

	//initialize bullet speed and find the ration between x to y
	float ratioXY = m_BulletSpeed / (1 + gradient);
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	//set bullet direction
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}
	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}
	//set range and make sure bullet does not exceed it
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	//position bullet
	m_BulletShape.setPosition(m_Position);
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::Update(float elaspedTime)
{
	//update location
	m_Position.x += m_BulletDistanceX * elaspedTime;
	m_Position.y += m_BulletDistanceY * elaspedTime;

	//move to new location
	m_BulletShape.setPosition(m_Position);

	//check range to see if bullet needs to be despawned
	if (m_Position.x <m_MinX || m_Position.x> m_MaxX || m_Position.y <m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}
