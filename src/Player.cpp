#include "Player.h"
#include "Hero.h"
#include "Logger.hpp"

namespace heroes {

Player::Player(PlayerKinds kind) : kind_{kind} {}

void Player::addHero(Hero *hero) {
  heroes_.push_back(hero);
  // hero->setFlagKind(kind_);
}

void Player::removeHero(Hero *hero) {
  heroes_.erase(std::find(heroes_.begin(), heroes_.end(), hero));
}

Hero *Player::getFirstHeroOrNull() {
  if (!heroes_.empty()) {
    return heroes_.front();
  }
  return nullptr;
}

PlayerKinds Player::getKind() const { return kind_; }

} // namespace heroes
