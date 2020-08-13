#pragma once

#include "States.h"

namespace heroes {

class Application {
public:
  explicit Application(sf::RenderWindow &window);
  void play();

private:
  void registerStates();

  void draw();
  void update(sf::Time dt);
  void processEventInput();
  void processRealTimeInput();

private:
  sf::RenderWindow &window_;
  StateStack stateStack_;
};

} // namespace heroes
