#pragma once

#include "GUI.h"
#include "States.h"
#include "World.h"

namespace heroes {
class GameState : public State {
private:
  constexpr static float ratio = 0.8f;

public:
  GameState(StateStack &, State::Context);

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
  GameView view_;
  World world_;
  GUI::Panel container_;
};
} // namespace heroes
