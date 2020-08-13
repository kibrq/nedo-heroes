#include "GUI.h"
#include "Logger.hpp"

namespace heroes::GUI {

Button::Button() : Component() {}

Button::Button(sf::Vector2i size)
    : Component(size), shape_{sf::Vector2f(size)} {
  shape_.setFillColor(sf::Color::Red);
}

Button::Button(sf::Vector2i size, std::function<void()> callback)
    : Button(size) {
  setCallback(callback);
}

void Button::setSize(sf::Vector2i size) {
  Component::setSize(size);
  shape_.setSize(sf::Vector2f(size));
}

void Button::setCallback(std::function<void()> callback) {
  callback_ = callback;
}

bool Button::handleMouseButtonClicked(sf::Mouse::Button button,
                                      sf::Vector2i position) {
  if (button != sf::Mouse::Left || !isInside(position)) {
    return true;
  }
  callback_();
  return false;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape_, states);
}

} // namespace heroes::GUI
