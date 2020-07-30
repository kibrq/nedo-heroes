#include "Window.h"
#include "AbstractCore.h"
#include "Logger.hpp"
#include "MainGamePanel.h"
#include "Textures.h"
#include <array>
#include <map>

namespace heroes {

Window::Window() : window_(sf::VideoMode(1500, 750), "heroes") {
  textures::loadTextures();
  panels_.emplace_back(std::make_unique<MainGamePanel>(1500, 750));
}

void Window::play() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      }
    }
    handleRealTimeInput();
    window_.clear();
    panels_.back()->update(sf::milliseconds(1));
    for (auto &pans : panels_) {
      window_.draw(*pans);
    }

    window_.display();
  }
}

void Window::handleRealTimeInput() {
  static std::array<bool, sf::Mouse::ButtonCount> _isButtonPressedArray_;
  std::size_t buttonNum = 0;
  for (auto &pressed : _isButtonPressedArray_) {
    auto button = static_cast<sf::Mouse::Button>(buttonNum);
    auto _pressed_ = sf::Mouse::isButtonPressed(button);
    auto position = sf::Mouse::getPosition(window_);
    if (_pressed_) {
      panels_.back()->handleMouseButtonPressed(button, position);
    } else if (pressed) {
      panels_.back()->handleMouseButtonReleased(button, position);
      panels_.back()->handleMouseButtonClicked(button, position);
    }
    pressed = _pressed_, ++buttonNum;
  }

  static std::array<bool, sf::Keyboard::KeyCount> _isKeyPressedArray_;
  std::size_t keyNum = 0;
  for (auto &pressed : _isKeyPressedArray_) {
    auto key = static_cast<sf::Keyboard::Key>(keyNum);
    auto _pressed_ = sf::Keyboard::isKeyPressed(key);
    if (_pressed_) {
      panels_.back()->handleKeyPressed(key);
    } else if (pressed) {
      panels_.back()->handleKeyReleased(key);
      panels_.back()->handleKeyClicked(key);
    }
    pressed = _pressed_, ++keyNum;
  }
}

} // namespace heroes
