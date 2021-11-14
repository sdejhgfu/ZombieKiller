#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type)
{
	m_Type = type;

	if (m_Type == 1)
	{
		m_Sprite = (Sprite)TextureHolder::GetTexture("graphics/health_pickup.png");

		m_Value = HEALTH_START_VALUE;
	}

	else
	{
		m_Sprite = (Sprite)TextureHolder::GetTexture("graphics/ammo_pickup.png");

		m_Value = AMMO_START_VALUE;
	}

	m_Sprite.setOrigin(25, 25);

	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;

	srand(int(time(0) / m_Type));
}

void Pickup::setArena(IntRect arena)
{
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;
	Spawn();
}

void Pickup::Spawn()
{
	//pick a random location
	int x = (rand() % m_Arena.width);
	srand(int(time(0) / m_Type));
	int y = (rand() % m_Arena.height);

	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_Sprite.setPosition(x, y);

}

FloatRect Pickup::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Pickup::getSprite()
{
	return m_Sprite;
}


void Pickup::Update(float elaspedTime)
{
	//check if pickup alive or not
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elaspedTime;
	}
	else
	{
		m_SecondsSinceDespawn += elaspedTime;
	}

	//spawn/despawn accordingly	
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned)
	{
		m_Spawned = false;
		m_SecondsSinceDespawn = 0;
	}

	if (m_SecondsSinceDespawn > m_SecondsToWait && !m_Spawned)
	{
		Spawn();
	}
}

bool Pickup::isSpawned()
{
	return m_Spawned;
}

int Pickup::GotIt()
{
	m_Spawned = false;
	m_SecondsSinceDespawn = 0;
	return	m_Value;
}

void Pickup::Upgrade()
{
	if (m_Type == 1)
	{
		m_Value += (HEALTH_START_VALUE * .5f);
	}
	else
	{
		m_Value += (AMMO_START_VALUE * .5f);
	}

	//make spawns longer/stronger
	m_SecondsToLive += START_SECONDS_TO_LIVE / 10;
	m_SecondsToWait -= (START_WAIT_TIME / 10);

}
