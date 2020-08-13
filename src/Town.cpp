#include "Town.h"
#include "Hero.h"
#include "Logger.hpp"
#include "Textures.h"
#include "World.h"
#include <cassert>

namespace heroes {
Town::Town(TownKinds kind, PlayerKinds owner, sf::Vector2i location,
           World &world)
    : OwnableEntity(owner, location, world), kind_{kind} {

  auto &res = textures::TownResourcesHolder();
  onMap_.setTexture(res.get(kind_).onMapPic_);
}

Town::Town(TownKinds kind, sf::Vector2i location, World &world)
    : Town(kind, PlayerKinds::None, location, world) {}

void Town::heroVisited(Hero *hero) {}

TownKinds Town::getKind() const { return kind_; }

void Town::drawCurrent(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(onMap_, states);
}

void Town::onSetParent() {
  auto bounds = onMap_.getLocalBounds();
  onMap_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  auto vec = parent_->getCenter();
  move(vec.x, vec.y / 2);
}

} // namespace heroes
