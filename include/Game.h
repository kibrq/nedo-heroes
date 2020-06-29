#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace heroes {

struct Panel;

class Game {
public:
  Game();
  ~Game();
  void play();

private:
  sf::RenderWindow window_;
  std::vector<std::unique_ptr<Panel>> panels_;
};
} // namespace heroes
