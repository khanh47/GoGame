#include "ResourceManager.h"
#include "raylib.h"
#include <iostream>

ResourceManager &ResourceManager::getInstance() {
	static ResourceManager instance; // Created only once
	return instance;								 // Always returns same instance
}

void ResourceManager::_preLoadTexture2D(const std::string &filename, const std::string &alias) {
	if (_textures.find(filename) != _textures.end()) {
		std::cerr << "Texture already loaded: " << filename << std::endl;
		return;
	}

	Texture2D texture = LoadTexture(filename.c_str());
	if (texture.id == 0) {
		throw std::runtime_error("Failed to load texture: " + filename);
	}

	_textures[filename] = texture;
	_MappingAliasToFilename[alias] = filename;
}

void ResourceManager::_preLoadFont(const std::string &filename, const std::string &alias) {
	if (_fonts.find(filename) != _fonts.end())
		return;

	const int fontPixelSize = 40; // increase if your window is large
	Font font = LoadFontEx(filename.c_str(), fontPixelSize, nullptr, 0);
	if (font.texture.id == 0)
		throw std::runtime_error("Failed to load font: " + filename);

	// turn off smoothing so text remains crisp at integer sizes
	SetTextureFilter(font.texture, 0);

	_fonts[filename] = font;
	_MappingAliasToFilename[alias] = filename;
}

void ResourceManager::_preLoadMusic(const std::string &filename, const std::string &alias) {
	if (_musics.find(filename) != _musics.end())
		return;

	Music music = LoadMusicStream(filename.c_str());
	if (music.stream.buffer == nullptr) {
		throw std::runtime_error("Failed to load music: " + filename);
	}

	_musics[filename] = music;
	_MappingAliasToFilename[alias] = filename;
}

void ResourceManager::_preLoadSound(const std::string &filename, const std::string &alias) {
	if (_sounds.find(filename) != _sounds.end())
		return;

	Sound sound = LoadSound(filename.c_str());
	if (sound.frameCount == 0) {
		throw std::runtime_error("Failed to load sound: " + filename);
	}

	_sounds[filename] = sound;
	_MappingAliasToFilename[alias] = filename;
}

void ResourceManager::_unloadTexture2D(const std::string &alias) {
	auto it = _MappingAliasToFilename.find(alias);
	if (it != _MappingAliasToFilename.end()) {
		const std::string &filename = it->second;
		auto texIt = _textures.find(filename);
		if (texIt != _textures.end()) {
			UnloadTexture(texIt->second);
			_textures.erase(texIt);
		}
		_MappingAliasToFilename.erase(it);
	}
}

void ResourceManager::_unloadFont(const std::string &alias) {
	auto it = _MappingAliasToFilename.find(alias);
	if (it != _MappingAliasToFilename.end()) {
		const std::string &filename = it->second;
		auto fontIt = _fonts.find(filename);
		if (fontIt != _fonts.end()) {
			UnloadFont(fontIt->second);
			_fonts.erase(fontIt);
		}
		_MappingAliasToFilename.erase(it);
	}
}

// flyweight pattern implementation
Texture2D &ResourceManager::getTexture2D(const std::string &alias) {
	auto it = _MappingAliasToFilename.find(alias);
	if (it != _MappingAliasToFilename.end()) {
		const std::string &filename = it->second;
		auto texIt = _textures.find(filename);
		if (texIt != _textures.end()) {
			return texIt->second;
		} else {
			throw std::runtime_error("Texture not loaded: " + filename);
		}
	} else {
		throw std::runtime_error("Texture alias not found: " + alias);
	}
}

Font &ResourceManager::getFont(const std::string &alias) {
	auto it = _MappingAliasToFilename.find(alias);
	if (it != _MappingAliasToFilename.end()) {
		const std::string &filename = it->second;
		auto fontIt = _fonts.find(filename);
		if (fontIt != _fonts.end()) {
			return fontIt->second;
		} else {
			throw std::runtime_error("Font not found: " + filename);
		}
	} else {
		throw std::runtime_error("Font alias not found: " + alias);
	}
}

Music &ResourceManager::getMusic(const std::string &alias) {
	const std::string &filename = _MappingAliasToFilename.at(alias);
	return _musics.at(filename);
}

Sound &ResourceManager::getSound(const std::string &alias) {
	const std::string &filename = _MappingAliasToFilename.at(alias);
	return _sounds.at(filename);
}

ResourceManager::~ResourceManager() {
	// Unload all textures and fonts

	for (auto &p : _textures)
		UnloadTexture(p.second);
	for (auto &p : _fonts)
		UnloadFont(p.second);
	for (auto &p : _musics)
		UnloadMusicStream(p.second);
	for (auto &p : _sounds)
		UnloadSound(p.second);

	_textures.clear();
	_fonts.clear();
	_musics.clear();
	_sounds.clear();
	_MappingAliasToFilename.clear();
}

ResourceManager::ResourceManager() {
	_preLoadTexture2D("assets/background_image/info_panel.png", "info_panel");
	_preLoadTexture2D("assets/background_image/in-game_background.jpg", "in_game_background");
	_preLoadTexture2D("assets/background_image/main_menu_background.jpg", "main_menu_background");

	_preLoadTexture2D("assets/images/white_cartoon_stone.png", "white_cartoon_stone");
	_preLoadTexture2D("assets/images/black_cartoon_stone.png", "black_cartoon_stone");
	_preLoadTexture2D("assets/images/white_aesthetic_stone.png", "white_aesthetic_stone");
	_preLoadTexture2D("assets/images/black_aesthetic_stone.png", "black_aesthetic_stone");

	_preLoadTexture2D("assets/images/no_volume.png", "no_volume");
	_preLoadTexture2D("assets/images/low_volume.png", "low_volume");
	_preLoadTexture2D("assets/images/high_volume.png", "high_volume");

	_preLoadTexture2D("assets/images/ready.png", "ready_image");

	_preLoadFont("assets/fonts/GozaruDemo.ttf", "GozaruDemo");
	_preLoadFont("assets/fonts/NinjaKageDemo-Regular.otf", "NinjaKageDemo");
	_preLoadFont("assets/fonts/Boldonse-Regular.ttf", "Boldonse");

	_preLoadMusic("assets/musics/Hikaru no Go - OST 1 - 10 - Yuu.mp3", "main_music");
	_preLoadMusic("assets/musics/Hikaru no Go - OST 2 - 17 - Unmei No Ashioto.mp3", "in_game_music");

	_preLoadSound("assets/sounds/placing_stones.mp3", "placing_stones");
	_preLoadSound("assets/sounds/winning_sound.mp3", "winning_sound");
}
