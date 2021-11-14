#pragma once
#include "SFML/Graphics.hpp"
#include "Zombie.h"
using namespace sf;
int CreateBackgroundArena(VertexArray& rVA, IntRect arena);
Zombie* CreateHorde(int numZombies, IntRect arena);