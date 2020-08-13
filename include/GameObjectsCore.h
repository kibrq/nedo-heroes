#pragma once

#include "AbstractCore.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

namespace heroes {

struct SimpleEntity : SceneNode {
  SimpleEntity(sf::Vector2i loaction);
  virtual ~SimpleEntity();

  sf::Vector2i getLocation() const;
  void setLocation(sf::Vector2i);

protected:
  sf::Vector2i location_;
};

class World;

struct Entity : SimpleEntity {
  Entity(sf::Vector2i location, World &world);

protected:
  World &world_;
};

struct OwnableEntity : Entity {
  OwnableEntity(sf::Vector2i location, World &world);
  OwnableEntity(PlayerKinds kind, sf::Vector2i location, World &world);

  PlayerKinds getOwner() const;
  void setOwner(PlayerKinds);

protected:
  PlayerKinds owner_{PlayerKinds::None};
};

class Hero;
struct Visitable {
  virtual ~Visitable();

  virtual void heroVisited(Hero *hero) = 0;
};

} // namespace heroes
