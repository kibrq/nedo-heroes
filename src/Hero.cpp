#include "Hero.h"

namespace heroes {

Hero::Hero(sf::Vector2i pos, const sf::Texture &texture)
    : position_{pos}, sprite_{texture} {}

sf::Vector2i Hero::getPosition() const { return position_; }
void Hero::setPosition(sf::Vector2i position) { position_ = position; }

void Hero::drawCurrent(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(sprite_, states);
}

void Hero::updateCurrent(sf::Time dt) {}

} // namespace heroes
