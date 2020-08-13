#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace heroes::GUI {

struct Component : sf::Drawable, sf::Transformable, private sf::NonCopyable {
  Component() = default;
  Component(sf::Vector2i size) : size_{size} {}
  virtual ~Component() {}

  const Component *getParent() const { return parent_; }
  void setParent(Component *comp) { parent_ = comp; }

  sf::Vector2i getSize() const { return size_; }
  void setSize(sf::Vector2i size) { size_ = size; }

  sf::Vector2f getAbsolutePosition() const {
    sf::Vector2f result{0, 0};
    for (auto *current = this; current != nullptr; current = current->parent_) {
      result += current->getPosition();
    }
    return result;
  }

  bool isInside(sf::Vector2i position) {
    sf::IntRect rect{sf::Vector2i(getAbsolutePosition()), size_};
    return rect.contains(position);
  }

  virtual bool handleMouseButtonClicked(sf::Mouse::Button, sf::Vector2i) {
    return true;
  }
  virtual bool handleMouseButtonPressed(sf::Mouse::Button, sf::Vector2i) {
    return true;
  }
  virtual bool handleMouseButtonReleased(sf::Mouse::Button, sf::Vector2i) {
    return true;
  }

  virtual bool handleMouseMoved(sf::Vector2i) { return true; }
  virtual bool handleMouseWheelScrolled(int delta, sf::Vector2i) {
    return true;
  }

  virtual bool handleKeyClicked(sf::Keyboard::Key) { return true; }
  virtual bool handleKeyPressed(sf::Keyboard::Key) { return true; }
  virtual bool handleKeyReleased(sf::Keyboard::Key) { return true; }

protected:
  Component *parent_{nullptr};
  sf::Vector2i size_;
};

struct Panel : Component {
  Panel();
  Panel(sf::Vector2i size);

  void pack(std::shared_ptr<Component> component);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
  bool handleMouseButtonClicked(sf::Mouse::Button, sf::Vector2i) override;
  bool handleMouseButtonPressed(sf::Mouse::Button, sf::Vector2i) override;
  bool handleMouseButtonReleased(sf::Mouse::Button, sf::Vector2i) override;

  bool handleMouseMoved(sf::Vector2i) override;
  bool handleMouseWheelScrolled(int, sf::Vector2i) override;

  bool handleKeyClicked(sf::Keyboard::Key) override;
  bool handleKeyPressed(sf::Keyboard::Key) override;
  bool handleKeyReleased(sf::Keyboard::Key) override;

private:
  std::vector<std::shared_ptr<Component>> children_;
};

struct Button : Component {
  Button();
  Button(sf::Vector2i size);
  Button(sf::Vector2i size, std::function<void()> callback);

  void setCallback(std::function<void()> callback);

  void setSize(sf::Vector2i size);

  bool handleMouseButtonClicked(sf::Mouse::Button button,
                                sf::Vector2i position) override;

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
  std::function<void()> callback_;
  sf::RectangleShape shape_;
};

} // namespace heroes::GUI
