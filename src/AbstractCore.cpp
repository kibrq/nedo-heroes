#include "AbstractCore.h"
#include "Logger.hpp"
#include <cassert>

namespace heroes {

void SceneNode::onCommand(Command *command, sf::Time dt) {
  command->doAction(this, dt);
  for (auto &child : children_) {
    child->onCommand(command, dt);
  }
}

Animation::Animation(sf::Time duration, bool repetative)
    : duration_{duration}, needRepeat_{repetative} {}

bool Animation::update(sf::Time dt) {
  auto timePerFrame = duration_ / static_cast<float>(frames_.size());
  bool result = false;
  if (isStopped) {
    return result;
  }
  elapsedTime_ += dt;
  if (elapsedTime_ >= timePerFrame) {
    elapsedTime_ -= timePerFrame;
    ++currentFrame_;
    if (currentFrame_ == frames_.size()) {
      result = true;
      currentFrame_ = 0;
      if (!needRepeat_) {
        stop();
      }
    }
  }
  return result;
}

void Animation::stop() {
  isStopped = true;
  elapsedTime_ = sf::Time::Zero;
}

void Animation::reset() {
  stop();
  currentFrame_ = 0;
}

void Animation::resume() { isStopped = false; }

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(frames_[currentFrame_], states);
}

Panel::Panel(int width, int height) : Panel(sf::Vector2i(width, height)) {}
Panel::Panel(sf::Vector2i size) : size_{size} {}

#define HANDLE_MOUSE_BUTTON(NAME)                                              \
  bool Panel::handleMouseButton##NAME(sf::Mouse::Button button,                \
                                      sf::Vector2i position) {                 \
    if (!isInBounds(position)) {                                               \
      return true;                                                             \
    }                                                                          \
    if (!handleMouseButton##NAME##Current(button, position)) {                 \
      return false;                                                            \
    }                                                                          \
    for (auto &child : children_) {                                            \
      if (child->handleMouseButton##NAME(button, position)) {                  \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
    return true;                                                               \
  }

HANDLE_MOUSE_BUTTON(Clicked)
HANDLE_MOUSE_BUTTON(Pressed)
HANDLE_MOUSE_BUTTON(Released)

bool Panel::handleMouseWheelScrolled(int delta, sf::Vector2i position) {
  if (!isInBounds(position)) {
    return true;
  }
  if (!handleMouseWheelScrolledCurrent(delta, position)) {
    return false;
  }
  for (auto &child : children_) {
    if (child->handleMouseWheelScrolled(delta, position)) {
      return false;
    }
  }
  return true;
}

bool Panel::handleMouseMoved(sf::Vector2i position) {
  if (!isInBounds(position)) {
    return true;
  }
  if (!handleMouseMovedCurrent(position)) {
    return false;
  }
  for (auto &child : children_) {
    if (child->handleMouseMoved(position)) {
      return false;
    }
  }
  return true;
}

#define HANDLE_KEY(NAME)                                                       \
  bool Panel::handleKey##NAME(sf::Keyboard::Key key) {                         \
    if (!handleKey##NAME##Current(key)) {                                      \
      return false;                                                            \
    }                                                                          \
    for (auto &child : children_) {                                            \
      if (child->handleKey##NAME(key)) {                                       \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
    return true;                                                               \
  }

HANDLE_KEY(Clicked)
HANDLE_KEY(Pressed)
HANDLE_KEY(Released)

sf::Vector2i Panel::getRelativePosition(int x, int y) const {
  return getRelativePosition(sf::Vector2i(x, y));
}

sf::Vector2i Panel::getRelativePosition(sf::Vector2i v) const {
  return v - getAbsolutePosition();
}

bool Panel::isInBounds(int x, int y) const {
  return isInBounds(sf::Vector2i(x, y));
}

bool Panel::isInBounds(sf::Vector2i absolutePosition) const {
  auto relativePosition = getRelativePosition(absolutePosition);
  auto isInRange = [](auto left, auto right, auto test) {
    return left <= test && test < right;
  };
  return isInRange(0, size_.x, relativePosition.x) &&
         isInRange(0, size_.y, relativePosition.y);
}

} // namespace heroes
