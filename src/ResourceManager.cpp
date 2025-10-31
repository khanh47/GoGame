#include "ResourceManager.h"
#include "raylib.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;   // Created only once
    return instance;                   // Always returns same instance
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
    if (_fonts.find(filename) != _fonts.end()) return;

    const int fontPixelSize = 40; // increase if your window is large
    Font font = LoadFontEx(filename.c_str(), fontPixelSize, nullptr, 0);
    if (font.texture.id == 0) throw std::runtime_error("Failed to load font: " + filename);

    // turn off smoothing so text remains crisp at integer sizes
    SetTextureFilter(font.texture, 0);

    _fonts[filename] = font;
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
Texture2D& ResourceManager::getTexture2D(const std::string &alias) {
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

Font& ResourceManager::getFont(const std::string &alias) {
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

ResourceManager::~ResourceManager() {
    // Unload all textures and fonts
    for (const auto &pair : _textures) {
        UnloadTexture(pair.second);
    }
    _textures.clear();

    for (const auto &pair : _fonts) {
        UnloadFont(pair.second);
    }
    _fonts.clear();

    _MappingAliasToFilename.clear();
}

ResourceManager::ResourceManager() {    
    _preLoadTexture2D("assets/background_image/info_panel.png", "info_panel");
    _preLoadFont("assets/fonts/GozaruDemo.ttf", "GozaruDemo");
    _preLoadTexture2D("assets/images/Theme_0/ready.png", "ready_image");
    // ... all your preloads ...
}