#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture & TextureHolder::GetTexture(std::string const & filename)
{
	//get reference to texture dictionary
	auto& m = m_s_Instance->m_Texture;

	//create iterator to hold key value pair (kvp)
	//search for correct kvp using passed file name and return it
	auto keyValuePair = m.find(filename);
	
	//auto is equivalent of map<string, Texture>::iterator

	//check to see if pair found
	if (keyValuePair != m.end())
	{
		//yes, return texture (second part of kvp)
		return keyValuePair->second;
	}
	else
	{
		//file name not found
		//create new kvp using the filename
		auto& texture = m[filename];
		//load texture
		texture.loadFromFile(filename);

		//return texture that is now in the kvp
		return texture;
	}
}
