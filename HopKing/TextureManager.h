#pragma once
#include <string>
#include <unordered_map>
class Texture;
class MenuStyle;

class TextureManager final
{
public:
	static TextureManager* Get();
	TextureManager();
	~TextureManager();

	TextureManager(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;

	void Initialize();

	void AddTexture(const std::string& key, Texture* texture);
	Texture* GetTexture(const std::string& key);
	Texture* GetFileTexture(const std::string& filePath, const std::string& fileName);
	Texture* GetTextTexture(const std::string& text, MenuStyle* pMenuStyle);


private:
	std::unordered_map<std::string, Texture*> m_pTextureMap;


};
