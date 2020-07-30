#include "Hero.h"
#include "Logger.hpp"
#include "Textures.h"
#include "World.h"
#include <cassert>
#include <cmath>

namespace heroes {
namespace {
float angleBetween(sf::Vector2i v1, sf::Vector2i v2) {
  auto vecAbs = [](sf::Vector2i v) { return std::sqrt(v.x * v.x + v.y * v.y); };
  float cs = ((v1.x * v2.x + v1.y * v2.y) / (vecAbs(v1) * vecAbs(v2)));
  float sn = ((v1.x * v2.y - v1.y * v2.x) / (vecAbs(v1) * vecAbs(v2)));
  if (cs >= 0 && sn >= 0) {
    return std::acos(cs);
  } else if (cs >= 0 && sn < 0) {
    return std::asin(sn);
  } else if (cs < 0 && sn >= 0) {
    return std::acos(cs);
  } else {
    return -std::acos(cs);
  }
}

float angleBetweenNorm(sf::Vector2i v) {
  return angleBetween(sf::Vector2i(0, -1), v);
}

} // namespace

textures::PathArrowKinds getIdentifier(sf::Vector2i v1, sf::Vector2i v2) {
  auto approx = [](float ethalon, float val) {
    static float err = 0.5;
    return ethalon - err <= val && val <= ethalon + err;
  };
  float angle = angleBetween(v1, v2) * 180 / M_PI;
  bool left = angle >= 0;
  angle = std::abs(angle);
  if (approx(0, angle) || approx(180, angle)) {
    return textures::PathArrowKinds::Straight;
  } else if (approx(45, angle)) {
    return left ? textures::PathArrowKinds::Turn45Left
                : textures::PathArrowKinds::Turn45Right;
  } else if (approx(90, angle)) {
    return left ? textures::PathArrowKinds::Turn90Left
                : textures::PathArrowKinds::Turn90Right;
  } else if (approx(135, angle)) {
    return left ? textures::PathArrowKinds::Turn135Left
                : textures::PathArrowKinds::Turn135Right;
  } else {
    assert(false && "unreacheable");
  }
}

Hero::Path::Edge::Edge(std::optional<sf::Vector2i> previous,
                       sf::Vector2i current,
                       std::optional<sf::Vector2i> further)
    : SimpleEntity(current) {
  std::optional<sf::Vector2i> toPrevious, toNext;

  if (previous) {
    toPrevious.emplace(previous.value() - current);
  }
  if (further) {
    toNext.emplace(further.value() - current);
  }
  direction_ = toPrevious ? -toPrevious.value() : sf::Vector2i(0, 0);
  initTexture(toPrevious, toNext);
  if (sprite_)
    initTransforms(toPrevious, toNext);
}

sf::Vector2i Hero::Path::Edge::getLocation() const { return location_; }
sf::Vector2i Hero::Path::Edge::getDirection() const { return direction_; }

void Hero::Path::Edge::initTexture(std::optional<sf::Vector2i> toPrevious,
                                   std::optional<sf::Vector2i> toNext) {
  if (!toPrevious) {
    return;
  }
  sprite_.emplace();
  if (!toNext) {
    sprite_->setTexture(textures::PathArrowResourcesHolder().get(
        textures::PathArrowKinds::Stop));
    return;
  }

  sprite_->setTexture(textures::PathArrowResourcesHolder().get(
      getIdentifier(toPrevious.value(), toNext.value())));
}

void Hero::Path::Edge::initTransforms(std::optional<sf::Vector2i> toPrevious,
                                      std::optional<sf::Vector2i> toNext) {
  auto bounds = sprite_->getLocalBounds();
  sprite_->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  move(sf::Vector2f(50.f, 50.f));
  if (!toNext) {
    return;
  }
  toPrevious.value() *= -1;
  rotate(angleBetweenNorm(toPrevious.value()) * 180 / M_PI);
}

void Hero::Path::Edge::drawCurrent(sf::RenderTarget &target,
                                   sf::RenderStates states) const {
  if (sprite_ && !hide_)
    target.draw(*sprite_, states);
}

void Hero::Path::Edge::toggleHide() { hide_ = !hide_; }

Hero::Path::Path(const std::deque<sf::Vector2i> &path, World &world)
    : world_{world} {
  for (std::size_t i = 0, size = path.size(); i < size; ++i) {
    std::optional<sf::Vector2i> previous, further;
    if (i != 0) {
      previous.emplace(path[i - 1]);
    }
    if (i != size - 1) {
      further.emplace(path[i + 1]);
    }
    auto edge = std::make_unique<Edge>(previous, path[i], further);
    edges_.push_back(edge.get());
    world_.addNode(std::move(edge), path[i]);
  }
}

Hero::Path::~Path() {
  for (auto &edge : edges_) {
    world_.removeNode(edge, edge->getLocation());
  }
}

sf::Vector2i Hero::Path::front() const { return edges_.front()->getLocation(); }

sf::Vector2i Hero::Path::direction() const {
  return edges_.front()->getDirection();
}

void Hero::Path::pop() {
  auto edge = edges_.front();
  world_.removeNode(edge, edge->getLocation());
  edges_.pop_front();
}

} // namespace heroes
