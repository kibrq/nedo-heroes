#pragma once

#include <vector>

namespace heroes {

enum class PlayerKinds { None, Red, Blue };

class Hero;

class Player {
public:
  Player(PlayerKinds kind);

  void addHero(Hero *);
  void removeHero(Hero *);

  Hero *getFirstHeroOrNull();

  PlayerKinds getKind() const;

private:
  PlayerKinds kind_;
  std::size_t id_;
  std::vector<Hero *> heroes_;
};
} // namespace heroes
