#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace heroes {

template <typename Identifier, class Resource> struct ResourceHolder {
public:
  ResourceHolder() = default;
  void load(const Identifier &id, const std::string &filename);
  void set(const Identifier &id, std::unique_ptr<Resource> res);

  Resource &get(const Identifier &id);
  const Resource &get(const Identifier &id) const;

private:
  std::map<Identifier, std::unique_ptr<Resource>> map_;
};

template <typename I, class R>
void ResourceHolder<I, R>::load(const I &id, const std::string &filename) {
  std::unique_ptr<R> res = std::make_unique<R>();
  res->loadFromFile(filename);
  set(id, std::move(res));
}

template <typename I, class R>
void ResourceHolder<I, R>::set(const I &id, std::unique_ptr<R> res) {
  map_.emplace(id, std::move(res));
}

template <typename I, class R> R &ResourceHolder<I, R>::get(const I &id) {
  return *map_[id];
}

template <typename I, class R>
const R &ResourceHolder<I, R>::get(const I &id) const {
  return *map_[id];
}

struct TextureArray {
  TextureArray() = default;

  void loadFromFiles(const std::vector<std::string> &filenames) {
    textures_.resize(filenames.size());
    std::size_t i = 0;
    for (const auto &filename : filenames) {
      textures_[i++].loadFromFile(filename);
    }
  }
  std::vector<sf::Sprite> getAsSprites() const {
    std::vector<sf::Sprite> result;
    for (const auto &tex : textures_) {
      result.emplace_back(tex);
    }
    return result;
  }

private:
  std::vector<sf::Texture> textures_;
};

} // namespace heroes
