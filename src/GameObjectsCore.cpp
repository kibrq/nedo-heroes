#include "GameObjectsCore.h"
#include "Logger.hpp"
#include <cassert>

namespace heroes {
SimpleEntity::SimpleEntity(sf::Vector2i location) : location_{location} {}

SimpleEntity::~SimpleEntity() = default;

sf::Vector2i SimpleEntity::getLocation() const { return location_; }
void SimpleEntity::setLocation(sf::Vector2i loc) { location_ = loc; }

Entity::Entity(sf::Vector2i location, World &world)
    : SimpleEntity(location), world_{world} {}

OwnableEntity::OwnableEntity(sf::Vector2i location, World &world)
    : Entity(location, world) {}

OwnableEntity::OwnableEntity(PlayerKinds kind, sf::Vector2i location,
                             World &world)
    : Entity(location, world), owner_{kind} {}

PlayerKinds OwnableEntity::getOwner() const { return owner_; }
void OwnableEntity::setOwner(PlayerKinds kind) { owner_ = kind; }

Visitable::~Visitable() = default;

} // namespace heroes
