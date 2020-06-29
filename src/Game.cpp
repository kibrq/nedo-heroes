#include "Game.h"
#include "MainGamePanel.h"

namespace heroes {

Game::Game() : window_(sf::VideoMode(1500, 750), "heroes") {
  panels_.emplace_back(std::make_unique<MainGamePanel>(1500, 750));
}
Game::~Game() = default;

void Game::play() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      }
    }
    window_.clear();
    for (auto &panel : panels_) {
      panel->update(sf::milliseconds(100));
    }
    for (auto &panel : panels_) {
      window_.draw(*panel);
    }
    panels_.back()->handleKeyboard();
    panels_.back()->handleEvent(event);
    window_.display();
  }
}

} // namespace heroes
