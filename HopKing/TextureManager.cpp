#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include "MenuStyle.h"


TextureManager* TextureManager::Get()
{
	static TextureManager* t = new TextureManager();

	return t;
}

TextureManager::TextureManager()
	:m_pTextureMap{}
{
	Initialize();
}

TextureManager::~TextureManager()
{
	for (std::pair<std::string, Texture*> itr : m_pTextureMap) {
		delete itr.second;
		itr.second = nullptr;
	}

	m_pTextureMap.clear();
}

void TextureManager::Initialize()
{

}

void TextureManager::AddTexture(const std::string& key, Texture* texture)
{
	m_pTextureMap.insert(std::make_pair(key, texture));
}

Texture* TextureManager::GetTexture(const std::string& key)
{
	return m_pTextureMap[key];
}

Texture* TextureManager::GetFileTexture(const std::string& filePath, const std::string& fileName)
{
	if (!m_pTextureMap.contains(fileName))
	{
		m_pTextureMap.insert(std::make_pair(fileName, new Texture(filePath + fileName)));
	}
	return m_pTextureMap[fileName];
}

Texture* TextureManager::GetTextTexture(const std::string& text, MenuStyle* pMenuStyle)
{
	if (!m_pTextureMap.contains(text))
	{
		m_pTextureMap.insert(std::make_pair(text, new Texture(text, pMenuStyle->GetFont(), pMenuStyle->GetFontSize(), pMenuStyle->GetTextColor())));
	}
	return nullptr;
}
