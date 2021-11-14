#include "Player.h"
#include <iostream>
#include "TextureHolder.h"

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
	

	m_InceasePercent = .2f;

	//set player texture
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player.png"));

	//make the center the origin
	m_Sprite.setOrigin(25, 25);
}

void Player::Spawn(IntRect arena, Vector2f resolution, int tilesize)
{
	//set player to middle of screen
	m_Position.x = arena.width / 2.f;
	m_Position.y = arena.height / 2.f;

	//copy details of arena
	m_Arena.left = arena.left;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;
	m_Arena.width = arena.width;

	//copy tile size
	m_TileSize = tilesize;

	//copy resolution
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::ResetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

bool Player::Hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 500)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
		return false;
}

Time Player::getLastHit()
{
	return m_LastHit;
}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

void Player::MoveLeft()
{
	m_LeftPressed = true;
}

void Player::MoveRight()
{
	m_RightPressed = true;
}

void Player::MoveUp()
{
	m_UpPressed = true;
}

void Player::MoveDown()
{
	m_DownPressed = true;
}

void Player::StopLeft()
{
	m_LeftPressed = false;
}

void Player::StopRight()
{
	m_RightPressed = false;
}

void Player::StopUp()
{
	m_UpPressed = false;
}

void Player::StopDown()
{
	m_DownPressed = false;
}

void Player::Update(float elaspedTime, Vector2i mousePosition)
{
	//moving the player
	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elaspedTime;
	}
	if (m_DownPressed)
	{
		m_Position.y+=m_Speed * elaspedTime;
	}
	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elaspedTime;
	}
	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elaspedTime;
	}

	m_Sprite.setPosition(m_Position);


	//check to make sure player stays in arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}
	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}
	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}
	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}
	
	//calc angle player facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2.f) * 180) /3.131f;

	m_Sprite.setRotation(angle);

}

void Player::UpgradeSpeed()
{
	//upgrade by fixed amount
	m_Speed += (START_SPEED*m_InceasePercent);
}

void Player::UpgradeHealth()
{
	m_MaxHealth += (START_HEALTH * m_InceasePercent);
}

void Player::IncreaseHealthLevel(int amount)
{
	m_Health += amount;
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

float Player::getHealth()
{

	return m_Health/m_MaxHealth;
}
