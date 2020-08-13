#include "BetweenTurnsState.h"

namespace heroes {
BetweenTurnsState::BetweenTurnsState(StateStack &stack, Context context)
    : State(stack, context) {

  background_.setSize(sf::Vector2f(context.window_.getSize()));
}

void BetweenTurnsState::draw() { getContext().window_.draw(background_); }
void BetweenTurnsState::update(sf::Time dt) {}

bool BetweenTurnsState::handleMouseButtonClicked(sf::Mouse::Button,
                                                 sf::Vector2i) {
  requestStackPop();
  return false;
}

bool BetweenTurnsState::handleMouseButtonPressed(sf::Mouse::Button,
                                                 sf::Vector2i) {
  return true;
}

bool BetweenTurnsState::handleMouseButtonReleased(sf::Mouse::Button,
                                                  sf::Vector2i) {
  return true;
}

bool BetweenTurnsState::handleMouseMoved(sf::Vector2i) { return true; }
bool BetweenTurnsState::handleMouseWheelScrolled(int, sf::Vector2i) {
  return true;
}

bool BetweenTurnsState::handleKeyClicked(sf::Keyboard::Key) { return true; }

bool BetweenTurnsState::handleKeyPressed(sf::Keyboard::Key) { return true; }
bool BetweenTurnsState::handleKeyReleased(sf::Keyboard::Key) { return true; }

} // namespace heroes
