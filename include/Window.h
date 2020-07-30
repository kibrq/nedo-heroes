#pragma once

#include "AbstractCore.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>

class Panel;

namespace heroes {

class Window {
public:
  Window();
  void play();

private:
  void handleRealTimeInput();

private:
  sf::RenderWindow window_;
  std::deque<std::unique_ptr<Panel>> panels_;
};
} // namespace heroes
