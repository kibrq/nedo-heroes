#pragma once

#include "States.h"

namespace heroes {

class BetweenTurnsState : public State {
public:
  BetweenTurnsState(StateStack &, State::Context);

  void draw() override;
  void update(sf::Time dt) override;

  bool handleMouseButtonClicked(sf::Mouse::Button, sf::Vector2i) override;
  bool handleMouseButtonPressed(sf::Mouse::Button, sf::Vector2i) override;
  bool handleMouseButtonReleased(sf::Mouse::Button, sf::Vector2i) override;

  bool handleMouseMoved(sf::Vector2i) override;
  bool handleMouseWheelScrolled(int, sf::Vector2i) override;

  bool handleKeyClicked(sf::Keyboard::Key) override;
  bool handleKeyPressed(sf::Keyboard::Key) override;
  bool handleKeyReleased(sf::Keyboard::Key) override;

private:
  sf::RectangleShape background_;
};

} // namespace heroes
