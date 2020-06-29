#include "MainGamePanel.h"
#include <iostream>

namespace heroes {
HelperGamePanel::HelperGamePanel(sf::Vector2i size, Panel *parent)
    : Panel(size, parent) {}

void HelperGamePanel::drawCurrent(sf::RenderTarget &target,
                                  sf::RenderStates states) const {
  target.draw(*world_, states);
}

void HelperGamePanel::handleEventCurrent(sf::Event &event) {
  world_->handleEvent(event, getAbsolutePosition());
}

void HelperGamePanel::handleKeyboardCurrent() { world_->handleKeyboard(); }

void HelperGamePanel::updateCurrent(sf::Time dt) { world_->update(dt); }

GameInfoPanel::GameInfoPanel(sf::Vector2i size, Panel *parent)
    : Panel(size, parent) {
  button_.setRadius(100);
  button_.setFillColor(sf::Color::Red);
  background_.setSize(sf::Vector2<float>(size_));
  background_.setFillColor(sf::Color::Yellow);
}

void GameInfoPanel::drawCurrent(sf::RenderTarget &target,
                                sf::RenderStates states) const {
  target.draw(background_, states);
  target.draw(button_, states);
}

void GameInfoPanel::handleEventCurrent(sf::Event &event) {}

MainGamePanel::MainGamePanel(int width, int height)
    : Panel(width, height), world_(width * ratio, height) {

  std::unique_ptr<HelperGamePanel> helperPanel =
      std::make_unique<HelperGamePanel>(sf::Vector2i(width * ratio, height),
                                        this);
  helperPanel->setWorld(&world_);
  std::unique_ptr<GameInfoPanel> infoPanel = std::make_unique<GameInfoPanel>(
      sf::Vector2i(width * (1 - ratio), height), this);
  infoPanel->setWorld(&world_);
  infoPanel->move(sf::Vector2f(width * ratio, 0));
  children_.push_back(std::move(helperPanel));
  children_.push_back(std::move(infoPanel));
}

MainGamePanel::MainGamePanel(sf::Vector2i size)
    : MainGamePanel(size.x, size.y) {}

} // namespace heroes
