#include "ZombieKillerh.h"
#include "Zombie.h"

Zombie * CreateHorde(int numZombies, IntRect arena)
{
	Zombie* zombies = new Zombie[numZombies];
	int maxY = arena.height - 20;
	int maxX = arena.width - 20;
	int minY = arena.top + 20;
	int minX = arena.left + 20;
	for (int i = 0; i < numZombies; i++)
	{
		//pick where to spawn the zombie
		srand((int)time(0)*i);
		int side = (rand() % 4);
		float x, y;
		switch (side)
		{
		case 0:
			//left
			x = minX;
			y = (rand() % maxY) + minY;
			break;
		case 1:
			//right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;
		case 2:
			//top
			y = minY;
			x = (rand() % maxX) + minX;
			break;
		case 3:
			//bottom
			y = maxY;
			x = (rand() % maxX) + minX;
			break;
		}
		//decide type
		srand((int)time(0)*i * 2);
		int type = rand() % 3;
		//add zombie to array
		zombies[i].Spawn(x, y, type, i);

	}
	return zombies;
}