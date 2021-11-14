#include "ZombieKillerh.h"
#include "SFML/Graphics.hpp"
int CreateBackgroundArena(VertexArray& rVA, IntRect arena)
{
	//Anything done to rVA 
	//is done to the actual object in main

	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	//set primitive type
	rVA.setPrimitiveType(Quads);

	//set size of the vertex array
	rVA.resize(worldHeight *worldWidth * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
		//position each vert in quad
			rVA[currentVertex + 0].position = Vector2f(w*TILE_SIZE, h*TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f(w*TILE_SIZE + TILE_SIZE, h*TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f(w*TILE_SIZE + TILE_SIZE, h*TILE_SIZE + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w*TILE_SIZE, h * TILE_SIZE + TILE_SIZE);


			//define poistion in texture for current quad
			//grass/bush/trees etc
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
			{
				rVA[currentVertex + 0].texCoords = Vector2f(0, TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else
			{
				//not wall obstacle piece
				//use floor texture
				srand(int(time(0)) + h * w - h);
				int mOrG = rand() % TILE_TYPES;
				int verticalOffset = mOrG * TILE_SIZE;

				rVA[currentVertex + 0].texCoords = Vector2f(0, verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
			}

		currentVertex += VERTS_IN_QUAD;
		}
	}


	return TILE_SIZE;
}


