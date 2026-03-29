#pragma once
#include <windows.h>

#include <cstdint>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

struct MusicResource {
  sf::Music music;
  std::vector<std::uint8_t> data;
};
class Loader {
  static void* loadResourceData(int resourceId,
                                std::vector<std::uint8_t>& outData) {
    HMODULE hModule = GetModuleHandle(NULL);
    if (!hModule) return nullptr;
    HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(resourceId), RT_RCDATA);
    if (!hRes) return nullptr;
    HGLOBAL hResData = LoadResource(hModule, hRes);
    if (!hResData) return nullptr;
    void* pData = LockResource(hResData);
    if (!pData) return nullptr;
    DWORD size = SizeofResource(hModule, hRes);
    if (size == 0) return nullptr;
    outData.resize(static_cast<std::size_t>(size));
    std::memcpy(outData.data(), pData, size);
    return pData;
  }

 public:
  static bool loadFontFromResources(int resourceId, sf::Font& font) {
    std::vector<std::uint8_t> data;
    void* pData = loadResourceData(resourceId, data);
    if (!pData) return false;
    return font.openFromMemory(pData, static_cast<std::size_t>(data.size()));
  }

  static bool loadTextureFromResources(int resourceId, sf::Texture& texture) {
    std::vector<std::uint8_t> data;
    void* pData = loadResourceData(resourceId, data);
    if (!pData) return false;
    sf::Image image;
    if (!image.loadFromMemory(pData, static_cast<std::size_t>(data.size()))) {
      return false;
    }
    return texture.loadFromImage(image);
  }

  static std::unique_ptr<MusicResource> loadMusicFromResources(int resourceId) {
    std::vector<std::uint8_t> data;
    if (!loadResourceData(resourceId, data)) return nullptr;

    auto resource = std::make_unique<MusicResource>();
    resource->data = std::move(data);
    if (!resource->music.openFromMemory(resource->data.data(),
                                        resource->data.size())) {
      return nullptr;
    }
    return resource;
  }
};
