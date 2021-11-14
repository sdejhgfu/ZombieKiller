#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>
using namespace std;

Zombie::Zombie()
{
}

bool Zombie::Hit()
{
	m_Health--;
	//check to see if still alive
	if (m_Health <= 0)
	{
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}

	return false;
}

bool Zombie::IsAlive()
{
	return m_Alive;
}

void Zombie::Spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
	case 0:
		//bloater
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = 40;
		m_Health = 5;
		break;
	case 1:
		//chaser
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = 70;
		m_Health = 1;		
		break;
	case 2:
		//crawler
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = 20;
		m_Health = 3;
		break;
	}
	//set speed in range
	srand((int)time(0)*seed);
	float modifier = rand() % MAX_VARRIANCE + OFFSET;
	
	//express as faction so its between .7 and 1
	modifier /= 100;
	m_Speed *= modifier;

	//init start location
	m_Postion.x = startX;
	m_Postion.y = startY;

	//set origin to center of object
	m_Sprite.setOrigin(25, 25);

	//set its position
	m_Sprite.setPosition(m_Postion);
}

FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite()
{
	return m_Sprite;
}

void Zombie::Update(float elaspedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	//update zombie position based on player location
	if (playerX > m_Postion.x)
	{
		m_Postion.x += m_Speed * elaspedTime;

	}
	if (playerY > m_Postion.y)
	{
		m_Postion.y += m_Speed * elaspedTime;
	}
	if (playerX < m_Postion.x)
	{
		m_Postion.x -= m_Speed * elaspedTime;
	}
	if (playerY < m_Postion.y)
	{
		m_Postion.y -= m_Speed * elaspedTime;
	}
	//update sprite location
	m_Sprite.setPosition(m_Postion);

	//face the sprite correct direction
	float angle = (atan2(playerY - m_Postion.y, playerX - m_Postion.x) * 180 / 3.1415f);

	m_Sprite.setRotation(angle);
}
