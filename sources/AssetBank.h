#pragma once

#include "Util.h"
using Struct::Vect2I;

#include <string>
#include <unordered_map>
#include <map>

#include <filesystem>

struct TextureEntry
{
	Texture* pTexture;
	std::string name;
	bool multiple;
	Vect2I tileSize;
	Vect2I tileOffset;
	
	TextureEntry() = default;
	inline TextureEntry(Texture* texture, std::string _name, Vect2I _tileSize = Vect2I::zero, Vect2I _tileOffset = Vect2I::zero) :
		pTexture{ texture }, name{ _name }, multiple{ _tileSize != Vect2I::zero }, tileSize{ _tileSize }, tileOffset{ _tileOffset } {};

	inline ~TextureEntry() {
		UnloadTexture(*pTexture); delete pTexture; };
};

struct FontEntry
{
	Font* pFont;
	std::string name;

	FontEntry() = default;
	inline FontEntry(Font* font, std::string _name) :
		pFont{ font }, name{ _name } {};

	inline ~FontEntry() {
		UnloadFont(*pFont); delete pFont; };
};

struct SoundEntry
{
	Sound* pSound;
	std::string name;
	float length;

	SoundEntry() = default;
	inline SoundEntry(Sound* sound, std::string _name, float _length) :
		pSound{ sound }, name{ _name }, length{ _length } {};

	inline ~SoundEntry() {
		UnloadSound(*pSound); delete pSound; };
};


enum AssetType
{
	AssetDefault,
	AssetTexture,
	AssetFont,
	AssetSound,
};

class AssetBank
{
private:
	std::unordered_map<std::string, TextureEntry*> mLoadedTextures;
	std::unordered_map<std::string, FontEntry*> mLoadedFonts;
	std::unordered_map<std::string, SoundEntry*> mLoadedSounds;

	std::unordered_map<std::string, std::filesystem::path> mUnloadedTextures;
	std::unordered_map<std::string, std::filesystem::path> mUnloadedFonts;
	std::unordered_map<std::string, std::filesystem::path> mUnloadedSounds;

	Texture* mErrorTexture = nullptr;

	const std::string mResourcePath = "resources";

	static AssetBank* instance;

public:
	AssetBank() = default;
	~AssetBank() = default;

	void Init();

	void SearchAFolder(std::filesystem::path folderPath);
	void SearchAFolderFor(std::filesystem::path folderPath, AssetType forWhat);
	
	void FetchAll();
	void UnfetchAll();
	void LoadAll();
	void UnloadAll();
	
	inline void UnloadTextures() {
		for (auto& entry : mLoadedTextures) delete entry.second; mLoadedTextures.clear(); };

	inline TextureEntry* GetATexture(std::string textureName) {
		if (mLoadedTextures.count(textureName)) LoadATexture(textureName); return mLoadedTextures.at(textureName); };
	
	bool FetchATexture(std::filesystem::path texturePath);
	bool LoadATexture(std::string textureName);

	
	inline void UnloadFonts() {
		for (auto& entry : mLoadedFonts) delete entry.second; mLoadedFonts.clear();	};

	inline FontEntry* GetAFont(std::string fontName) {
		if (mLoadedFonts.count(fontName)) LoadAFont(fontName); return mLoadedFonts.at(fontName); };

	bool FetchAFont(std::filesystem::path fontPath);
	bool LoadAFont(std::string fontName);


	inline void UnloadSounds() {
		for (auto& entry : mLoadedSounds) delete entry.second; mLoadedSounds.clear(); };

	inline SoundEntry* GetASound(std::string soundName) {
		if (mLoadedSounds.count(soundName)) LoadATexture(soundName); return mLoadedSounds.at(soundName); };

	bool FetchASound(std::filesystem::path soundPath);
	bool LoadASound(std::string soundName);

	inline static AssetBank* GetInstance() {
		if (!instance) instance = new AssetBank(); return instance; };
};

