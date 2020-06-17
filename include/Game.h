#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace heroes {

class World;

class Game {
public:
  Game();
  ~Game();
  void play();

private:
  sf::RenderWindow window_;
  std::unique_ptr<World> world_;
};
} // namespace heroes
