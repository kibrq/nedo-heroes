#pragma once

#include <SFML/Graphics.hpp>

namespace heroes {

struct Panel : sf::Drawable, sf::Transformable, private sf::NonCopyable {
  Panel(int width, int height, Panel *parent = nullptr);
  Panel(sf::Vector2i size, Panel *parent = nullptr);

private:
  void draw(sf::RenderTarget &target,
            sf::RenderStates states) const override final;

public:
  void handleEvent(sf::Event &event);
  void handleKeyboard();
  void update(sf::Time dt);

protected:
  sf::IntRect getActualBoudns() const;
  sf::Transform getAbsoluteTransform() const;
  sf::Vector2i getAbsolutePosition() const;

protected:
  virtual void drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const {}
  virtual void drawCurrentPostOrder(sf::RenderTarget &target,
                                    sf::RenderStates states) const {}
  virtual void handleEventCurrent(sf::Event &event) {}
  virtual void handleEventCurrentPostOrder(sf::Event &event) {}
  virtual void handleKeyboardCurrent() {}
  virtual void handleKeyboardCurrentPostOrder() {}
  virtual void updateCurrent(sf::Time dt) {}
  virtual void updateCurrentPostOrder(sf::Time dt) {}

protected:
  sf::Vector2i size_;
  std::vector<std::unique_ptr<Panel>> children_;
  const Panel *parent_;
};

} // namespace heroes
