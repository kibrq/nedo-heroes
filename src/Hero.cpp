#include "Hero.h"
#include "Logger.hpp"
#include "World.h"
#include <cassert>

namespace heroes {

Hero::Hero(textures::HeroKinds kind, sf::Vector2i location, World &world)
    : Entity(location, world), moveEast_(sf::Time(sf::milliseconds(100))),
      moveWest_(sf::Time(sf::milliseconds(100))) {
  auto &res = textures::HeroResourcesHolder().get(kind);
  moveEast_.init(res, textures::MoveEast);
  moveWest_.init(res, textures::MoveWest);
  currentAnimation_ = &moveWest_;
}

Hero::HeroMovingAnimation::HeroMovingAnimation(sf::Time duration)
    : Animation(duration, true) {}

void Hero::HeroMovingAnimation::init(textures::HeroResources &res,
                                     textures::HeroGifKinds kind) {
  frames_ = res.gifs_.get(kind).getAsSprites();
}

void Hero::setLocation(sf::Vector2i location) {
  location_ = location;
  setPosition(sf::Vector2f(0, 0));
}

void Hero::drawCurrent(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(*currentAnimation_, states);
}

void Hero::updateCurrent(sf::Time dt) {
  if (!isMoving_) {
    return;
  }
  move(sf::Vector2f(path_->direction()));
  if (currentAnimation_->update(dt)) {
    world_.moveHero(this, path_->front());
    path_->pop();
    if (!isPathEmpty())
      chooseAnimationByDirection(path_->direction());
  }

  if (isPathEmpty()) {
    stopMoving();
  }
}

void Hero::chooseAnimationByDirection(sf::Vector2i direction) {
  currentAnimation_->stop();
  currentAnimation_ = direction.x > 0 ? &moveWest_ : &moveEast_;
  currentAnimation_->resume();
}

std::optional<sf::Vector2i> Hero::backOfPath() const {
  if (isPathEmpty()) {
    return std::nullopt;
  }
  return path_->edges_.back()->getLocation();
}

bool Hero::isPathEmpty() const { return !path_ || path_->edges_.empty(); }

void Hero::setPath(const std::deque<sf::Vector2i> &route) {
  auto newPath = std::make_unique<Path>(std::move(route), world_);
  swap(path_, newPath);
}

void Hero::startMoving() {
  isMoving_ = true;
  if (path_->front() == location_) {
    path_->pop();
  }
  chooseAnimationByDirection(path_->direction());
}

void Hero::stopMoving() {
  isMoving_ = false;
  currentAnimation_->stop();
}

void Hero::toggleHide() {
  if (isPathEmpty()) {
    return;
  }
  for (auto &edge : path_->edges_) {
    edge->toggleHide();
  }
}

} // namespace heroes
