#pragma once
#include "ResourceHolder.hpp"
#include <SFML/Graphics.hpp>

namespace heroes::textures {

enum HeroKinds { Cowboy };
enum HeroGifKinds { MoveEast, MoveWest };
enum HeroPhotoKinds { Profile };

struct HeroResources {
  ResourceHolder<HeroGifKinds, GifHolder> gifs_;
  ResourceHolder<HeroPhotoKinds, sf::Texture> photos_;
};

enum BackgroundKinds { Grass1 };

enum PathArrowKinds {
  Stop,
  Straight,
  Turn45Left,
  Turn45Right,
  Turn90Left,
  Turn90Right,
  Turn135Left,
  Turn135Right
};

struct Textures {
  static ResourceHolder<HeroKinds, HeroResources> &getHeroResourcesHolder() {
    static ResourceHolder<HeroKinds, HeroResources> res;
    return res;
  }

  static ResourceHolder<BackgroundKinds, sf::Texture> &
  getBackgroundResourcesHolder() {
    static ResourceHolder<BackgroundKinds, sf::Texture> res;
    return res;
  }

  static ResourceHolder<PathArrowKinds, sf::Texture> &
  getPathArrowResourcesHolder() {
    static ResourceHolder<PathArrowKinds, sf::Texture> res;
    return res;
  }

  static void loadTextures() {
    loadHeroesTextures();
    loadBackroundTextures();
    loadPathArrowTextures();
  }

private:
  static void loadHeroesTextures() {
    auto cowboyHero = std::make_unique<HeroResources>();
    auto moveWest1 = std::make_unique<GifHolder>();
    moveWest1->loadFromFiles({"../img/Heroes/Cowboy/MoveWest/0.png",
                              "../img/Heroes/Cowboy/MoveWest/1.png",
                              "../img/Heroes/Cowboy/MoveWest/2.png",
                              "../img/Heroes/Cowboy/MoveWest/3.png",
                              "../img/Heroes/Cowboy/MoveWest/4.png",
                              "../img/Heroes/Cowboy/MoveWest/5.png",
                              "../img/Heroes/Cowboy/MoveWest/6.png",
                              "../img/Heroes/Cowboy/MoveWest/7.png",
                              "../img/Heroes/Cowboy/MoveWest/8.png",
                              "../img/Heroes/Cowboy/MoveWest/9.png"});
    cowboyHero->gifs_.set(MoveWest, std::move(moveWest1));
    auto moveEast1 = std::make_unique<GifHolder>();
    moveEast1->loadFromFiles({"../img/Heroes/Cowboy/MoveEast/0.png",
                              "../img/Heroes/Cowboy/MoveEast/1.png",
                              "../img/Heroes/Cowboy/MoveEast/2.png",
                              "../img/Heroes/Cowboy/MoveEast/3.png",
                              "../img/Heroes/Cowboy/MoveEast/4.png",
                              "../img/Heroes/Cowboy/MoveEast/5.png",
                              "../img/Heroes/Cowboy/MoveEast/6.png",
                              "../img/Heroes/Cowboy/MoveEast/7.png",
                              "../img/Heroes/Cowboy/MoveEast/8.png",
                              "../img/Heroes/Cowboy/MoveEast/9.png"});
    cowboyHero->gifs_.set(MoveEast, std::move(moveEast1));
    getHeroResourcesHolder().set(Cowboy, std::move(cowboyHero));
  }

  static void loadBackroundTextures() {
    auto &res = getBackgroundResourcesHolder();
    res.load(Grass1, "../img/Background/Grass1.png");
  }

  static void loadPathArrowTextures() {
    auto &res = getPathArrowResourcesHolder();
    res.load(Stop, "../img/PathArrow/Stop.png");
    res.load(Straight, "../img/PathArrow/Straight.png");
    res.load(Turn45Left, "../img/PathArrow/Turn45Left.png");
    res.load(Turn45Right, "../img/PathArrow/Turn45Right.png");
    res.load(Turn90Left, "../img/PathArrow/Turn90Left.png");
    res.load(Turn90Right, "../img/PathArrow/Turn90Right.png");
    res.load(Turn135Left, "../img/PathArrow/Turn135Left.png");
    res.load(Turn135Right, "../img/PathArrow/Turn135Right.png");
  }
};

inline auto &HeroResourcesHolder() {
  return Textures::getHeroResourcesHolder();
}

inline auto &BackgroundResourcesHolder() {
  return Textures::getBackgroundResourcesHolder();
}

inline auto &PathArrowResourcesHolder() {
  return Textures::getPathArrowResourcesHolder();
}

inline void loadTextures() { Textures::loadTextures(); }

} // namespace heroes::textures
