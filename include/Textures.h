#pragma once

#include "Hero.h"
#include "Player.h"
#include "ResourceHolder.hpp"
#include "Town.h"
#include <SFML/Graphics.hpp>

namespace heroes::textures {

struct HeroResources {
  ResourceHolder<HeroMovingAnimationKinds, TextureArray> animations_;
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

struct TownResources {
  sf::Texture onMapPic_;
};

struct OwnerResources {
  TextureArray twoSideFlags_;
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

  static ResourceHolder<TownKinds, TownResources> &getTownResourcesHolder() {
    static ResourceHolder<TownKinds, TownResources> res;
    return res;
  }

  static ResourceHolder<PlayerKinds, OwnerResources> &
  getOwnerResourcesHolder() {
    static ResourceHolder<PlayerKinds, OwnerResources> res;
    return res;
  }

  static void loadTextures() {
    loadHeroesTextures();
    loadBackroundTextures();
    loadPathArrowTextures();

    loadTownTextures();
    loadFlagTextures();
  }

private:
  static void loadHeroesTextures() {
    auto cowboyHero = std::make_unique<HeroResources>();
    auto moveWest1 = std::make_unique<TextureArray>();
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
    cowboyHero->animations_.set(HeroMovingAnimationKinds::MoveWest,
                                std::move(moveWest1));
    auto moveEast1 = std::make_unique<TextureArray>();
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
    cowboyHero->animations_.set(HeroMovingAnimationKinds::MoveEast,
                                std::move(moveEast1));
    getHeroResourcesHolder().set(HeroKinds::Cowboy, std::move(cowboyHero));
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

  static void loadTownTextures() {
    auto &res = getTownResourcesHolder();
    auto castle = std::make_unique<TownResources>();
    castle->onMapPic_.loadFromFile("../img/Towns/Castle/OnMap.png");
    res.set(TownKinds::Castle, std::move(castle));
  }

  static void loadFlagTextures() {
    auto &res = getOwnerResourcesHolder();
    auto red = std::make_unique<OwnerResources>();
    red->twoSideFlags_.loadFromFiles(
        {"../img/Flags/Red/Left.png", "../img/Flags/Red/Right.png"});
    res.set(PlayerKinds::Red, std::move(red));
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

inline auto &TownResourcesHolder() {
  return Textures::getTownResourcesHolder();
}

inline auto &OwnerResourcesHolder() {
  return Textures::getOwnerResourcesHolder();
}

inline void loadTextures() { Textures::loadTextures(); }

} // namespace heroes::textures
