#pragma once

#include "SceneNode.h"
#include <SFML/Graphics.hpp>

namespace heroes {

class Hero : public SceneNode {
public:
  Hero(sf::Vector2i pos, const sf::Texture &texture);
  sf::Vector2i getPosition() const;
  void setPosition(sf::Vector2i);

private:
  void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;

private:
  sf::Vector2i position_;
  sf::Sprite sprite_;
};

} // namespace heroes
