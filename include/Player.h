#pragma once
#include "Hero.h"

namespace heroes {

class Player {
public:
  static std::size_t counter;
  static std::size_t getCounter() { return ++counter; }
  static void resetCounter() { counter = 0; }

public:
  void addHero(Hero *hero);
  void removeHero(Hero *hero);
  Hero *getFirstHeroOrNull();

private:
  std::size_t id_;
  std::vector<Hero *> heroes_;
};
} // namespace heroes
