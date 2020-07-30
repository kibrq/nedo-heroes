#include "MainGamePanel.h"
#include "Logger.hpp"
#include <cassert>

namespace heroes {
GameInfoPanel::GameInfoPanel(sf::Vector2i size)
    : GameInfoPanel(size.x, size.y) {}

GameInfoPanel::GameInfoPanel(int width, int height) : Panel(width, height) {}

void GameInfoPanel::drawCurrent(sf::RenderTarget &target,
                                sf::RenderStates states) const {}

MainGamePanel::MainGamePanel(int width, int height) : Panel(width, height) {

  LOG("HELLO");
  std::unique_ptr<WorldWrapperPanel> worldPanel =
      std::make_unique<WorldWrapperPanel>(width * ratio, height);
  LOG("HELLO");
  std::unique_ptr<GameInfoPanel> infoPanel =
      std::make_unique<GameInfoPanel>(width * (1 - ratio), height);

  infoPanel->move(sf::Vector2f(width * ratio, 0));
  this->attachChild(std::move(worldPanel));
  this->attachChild(std::move(infoPanel));
  LOG("HELLO");
}

MainGamePanel::MainGamePanel(sf::Vector2i size)
    : MainGamePanel(size.x, size.y) {}

} // namespace heroes
