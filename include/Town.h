#pragma once

#include "GameObjectsCore.h"

namespace heroes {

enum class TownKinds { Castle };

class Town final : public OwnableEntity, public Visitable {
public:
  Town(TownKinds kind, sf::Vector2i location, World &);
  Town(TownKinds kind, PlayerKinds owner, sf::Vector2i location, World &);

  void heroVisited(Hero *hero) override;

  TownKinds getKind() const;

private:
  void onSetParent() override;

  void drawCurrent(sf::RenderTarget &, sf::RenderStates states) const override;

private:
  TownKinds kind_;
  sf::Sprite onMap_;
};
} // namespace heroes
