#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include "raylib.h"

class ResourceManager {
public:
    static ResourceManager& getInstance();
    // Resources retrieval
    Texture2D& getTexture2D(const std::string &alias);
    Font& getFont(const std::string &alias);
    Music& getMusic(const std::string &alias);
    Sound& getSound(const std::string &alias);

private:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void _preLoadTexture2D(const std::string &alias, const std::string &filePath);
    void _preLoadFont(const std::string &alias, const std::string &filePath);
    void _preLoadMusic(const std::string &alias, const std::string &filePath);
    void _preLoadSound(const std::string &alias, const std::string &filePath);

    void _unloadTexture2D(const std::string &alias);
    void _unloadFont(const std::string &alias);
    void _unloadMusic(const std::string &alias);
    void _unloadSound(const std::string &alias);

    std::unordered_map<std::string, Texture2D> _textures;
    std::unordered_map<std::string, Font> _fonts;
    std::unordered_map<std::string, Music> _musics;
    std::unordered_map<std::string, Sound> _sounds;

    std::map<std::string, std::string> _MappingAliasToFilename;
};
