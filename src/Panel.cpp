#include "Panel.h"
#include <iostream>

namespace heroes {
Panel::Panel(int width, int height, Panel *parent)
    : Panel(sf::Vector2i(width, height), parent) {}
Panel::Panel(sf::Vector2i size, Panel *panel)
    : size_{size}, children_{}, parent_{panel} {}

sf::IntRect Panel::getActualBoudns() const {
  return sf::IntRect(getAbsolutePosition(), size_);
}

void Panel::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  drawCurrent(target, states);
  for (auto &child : children_) {
    child->draw(target, states);
  }
  drawCurrentPostOrder(target, states);
}

void Panel::handleEvent(sf::Event &event) {
  handleEventCurrent(event);
  for (auto &child : children_) {
    child->handleEvent(event);
  }
  handleEventCurrentPostOrder(event);
}

void Panel::handleKeyboard() {
  handleKeyboardCurrent();
  for (auto &child : children_) {
    child->handleKeyboard();
  }
  handleKeyboardCurrentPostOrder();
}

void Panel::update(sf::Time dt) {
  updateCurrent(dt);
  for (auto &child : children_) {
    child->update(dt);
  }
  updateCurrentPostOrder(dt);
}

sf::Transform Panel::getAbsoluteTransform() const {
  sf::Transform tranform = sf::Transform::Identity;
  for (const Panel *current = this; current->parent_ != nullptr;
       current = current->parent_)
    tranform *= current->getTransform();
  return tranform;
}

sf::Vector2i Panel::getAbsolutePosition() const {
  return sf::Vector2<int>(getAbsoluteTransform() * sf::Vector2f());
}

} // namespace heroes
