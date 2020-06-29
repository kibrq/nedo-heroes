#pragma once

#include "SceneNode.h"

namespace heroes {

class LandscapeObject : public SceneNode {
public:
  LandscapeObject(sf::Vector2i entry, const sf::Texture &texutre);
  virtual void heroEntered() = 0;

protected:
  sf::Vector2i entry_;
  sf::Sprite sprite_;
};

class ResourceGainer : public LandscapeObject {
public:
  ResourceGainer(sf::Vector2i entry, const sf::Texture &texture);

  void heroEntered() override;

private:
  void drawCurrent(sf::RenderTarget &target,
                   sf::RenderStates states) const override;
};

} // namespace heroes
