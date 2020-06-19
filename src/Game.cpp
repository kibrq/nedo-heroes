#include "Game.h"
#include "World.h"

namespace heroes {

Game::Game()
    : window_(sf::VideoMode(1000, 500), "heroes"),
      world_(std::make_unique<World>(window_, sf::IntRect{})) {}
Game::~Game() = default;

void Game::play() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      }
    }
    world_->handleEvent(event);
    world_->handleKeyboard();
    window_.clear();
    world_->update(sf::milliseconds(100));
    world_->draw();
    window_.display();
  }
}

} // namespace heroes
