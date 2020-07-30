#pragma once

#include "Commands.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace heroes {
template <typename T>
struct TreeLikeDrawable : sf::Transformable,
                          sf::Drawable,
                          private sf::NonCopyable {
public:
  void attachChild(std::unique_ptr<T>);
  std::unique_ptr<T> detachChild(const T *);
  std::unique_ptr<T> detachFromParent();

  T *getParent() const { return parent_; }

public:
  void update(sf::Time dt);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
  virtual void drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const = 0;

  virtual void updateCurrent(sf::Time dt) = 0;

protected:
  sf::Transform getAbsoluteTransform() const;
  sf::Vector2i getAbsolutePosition() const;

  std::vector<std::unique_ptr<T>> children_;
  T *parent_{nullptr};
};

struct SceneNode : TreeLikeDrawable<SceneNode> {
  SceneNode() = default;

  virtual std::optional<sf::Vector2f> getPositionOfCenter() {
    return std::nullopt;
  }

  void onCommand(Command *command, sf::Time dt);

protected:
  virtual void drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const override {}

  virtual void updateCurrent(sf::Time dt) override {}
};

struct Animation : sf::Drawable, sf::Transformable, private sf::NonCopyable {
  Animation(sf::Time duration, bool repetative);

public:
  bool update(sf::Time dt);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
  sf::Time duration_, elapsedTime_{sf::Time::Zero};

public:
  void stop();
  void reset();
  void resume();

private:
  bool isStopped{true};
  bool needRepeat_;

protected:
  std::vector<sf::Sprite> frames_;
  std::size_t currentFrame_{0};
};

struct Panel : TreeLikeDrawable<Panel> {
  Panel(int width, int height);
  Panel(sf::Vector2i size);

#define MOUSE_BUTTON_ACTION(NAME)                                              \
public:                                                                        \
  bool handleMouseButton##NAME(sf::Mouse::Button button,                       \
                               sf::Vector2i position);                         \
                                                                               \
protected:                                                                     \
  virtual bool handleMouseButton##NAME##Current(sf::Mouse::Button,             \
                                                sf::Vector2i) {                \
    return true;                                                               \
  }

  MOUSE_BUTTON_ACTION(Clicked)
  MOUSE_BUTTON_ACTION(Pressed)
  MOUSE_BUTTON_ACTION(Released)

#define KEYBOARD_ACTION(NAME)                                                  \
public:                                                                        \
  bool handleKey##NAME(sf::Keyboard::Key key);                                 \
                                                                               \
protected:                                                                     \
  virtual bool handleKey##NAME##Current(sf::Keyboard::Key key) { return true; }

  KEYBOARD_ACTION(Clicked)
  KEYBOARD_ACTION(Pressed)
  KEYBOARD_ACTION(Released)

#undef MOUSE_BUTTON_ACTION
#undef KEYBOARD_ACTION

public:
  bool handleMouseWheelScrolled(int delta, sf::Vector2i position);
  bool handleMouseMoved(sf::Vector2i newPosition);

protected:
  virtual bool handleMouseWheelScrolledCurrent(int delta,
                                               sf::Vector2i position) {
    return true;
  }

  virtual bool handleMouseMovedCurrent(sf::Vector2i newPosition) {
    return true;
  }

protected:
  sf::Vector2i getRelativePosition(int x, int y) const;

  sf::Vector2i getRelativePosition(sf::Vector2i absolutePosition) const;

  bool isInBounds(int x, int y) const;
  bool isInBounds(sf::Vector2i pos) const;

protected:
  virtual void drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const override {}
  virtual void updateCurrent(sf::Time dt) override {}

protected:
  sf::Vector2i size_;
};

} // namespace heroes

#include "AbstractCoreImpl.hpp"
