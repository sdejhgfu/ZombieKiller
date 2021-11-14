#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H


#include <SFML/Graphics.hpp>
#include <map>
using namespace sf;
using namespace std;

class TextureHolder 
{
private:
	//map object to hold pairs of strings and textures
	std::map<std::string, Texture> m_Texture;

	//pointer of the class and only one that exists
	static TextureHolder* m_s_Instance;
protected:

public:
	TextureHolder();
	static Texture& GetTexture(std::string const& filename);
};
#endif // !TEXTURE_HOLDER_H