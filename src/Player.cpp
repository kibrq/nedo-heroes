#include "Player.h"

namespace heroes {

std::size_t Player::counter = 0;

void Player::addHero(Hero *hero) { heroes_.push_back(hero); }

void Player::removeHero(Hero *hero) {
  heroes_.erase(std::find(heroes_.begin(), heroes_.end(), hero));
}

Hero *Player::getFirstHeroOrNull() {
  if (!heroes_.empty()) {
    return heroes_.front();
  }
  return nullptr;
}

} // namespace heroes
