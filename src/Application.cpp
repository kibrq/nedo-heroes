#include "Application.h"
#include "BetweenTurnsState.h"
#include "GameState.h"
#include "Logger.hpp"
#include "Textures.h"

namespace heroes {

Application::Application(sf::RenderWindow &window)
    : window_{window}, stateStack_{State::Context(window)} {
  registerStates();
  textures::loadTextures();

  stateStack_.pushState(States::Game);
}

void Application::registerStates() {
  stateStack_.registerState<GameState>(States::Game);
  stateStack_.registerState<BetweenTurnsState>(States::BetweenTurns);
}

void Application::play() {
  while (window_.isOpen()) {
    processEventInput();
    processRealTimeInput();
    draw();
    update(sf::milliseconds(1));
  }
}

void Application::draw() {
  window_.clear();
  stateStack_.draw();
  window_.display();
}

void Application::update(sf::Time dt) { stateStack_.update(dt); }

void Application::processEventInput() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      window_.close();
      break;
    case sf::Event::MouseMoved:
      stateStack_.handleMouseMoved(
          sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
      break;
    case sf::Event::MouseWheelScrolled:
      stateStack_.handleMouseWheelScrolled(
          event.mouseWheel.delta,
          sf::Vector2i(event.mouseWheel.x, event.mouseWheel.y));
      break;
    }
  }
}

void Application::processRealTimeInput() {
  static std::array<bool, sf::Mouse::ButtonCount> _isButtonPressedArray_;
  std::size_t buttonNum = 0;
  for (auto &pressed : _isButtonPressedArray_) {
    auto button = static_cast<sf::Mouse::Button>(buttonNum);
    auto _pressed_ = sf::Mouse::isButtonPressed(button);
    auto position = sf::Mouse::getPosition(window_);
    if (_pressed_) {
      stateStack_.handleMouseButtonPressed(button, position);
    } else if (pressed) {
      stateStack_.handleMouseButtonReleased(button, position);
      stateStack_.handleMouseButtonClicked(button, position);
    }
    pressed = _pressed_, ++buttonNum;
  }

  static std::array<bool, sf::Keyboard::KeyCount> _isKeyPressedArray_;
  std::size_t keyNum = 0;
  for (auto &pressed : _isKeyPressedArray_) {
    auto key = static_cast<sf::Keyboard::Key>(keyNum);
    auto _pressed_ = sf::Keyboard::isKeyPressed(key);
    if (_pressed_) {
      stateStack_.handleKeyPressed(key);
    } else if (pressed) {
      stateStack_.handleKeyReleased(key);
      stateStack_.handleKeyClicked(key);
    }
    pressed = _pressed_, ++keyNum;
  }
}

} // namespace heroes
