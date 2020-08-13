#include "GameState.h"
#include "Logger.hpp"

namespace heroes {

GameState::GameState(StateStack &stack, State::Context context)
    : State(stack, context), view_(sf::Vector2i(context.window_.getSize()),
                                   sf::FloatRect(0.f, 0.f, ratio, 1.f)),
      world_(view_, *this) {
  auto size = context.window_.getSize();
  int width = size.x, height = size.y;
  container_.setSize(sf::Vector2i(width * (1 - ratio), height));
  container_.move(sf::Vector2f(width * ratio, 0));
  auto button = std::make_shared<GUI::Button>(sf::Vector2i(70, 30));
  container_.pack(button);
  button->setCallback([this]() { world_.nextTurn(); });
  button->move(sf::Vector2f(width * (1 - ratio) / 2 - 35, height - 60));
}

void GameState::draw() {
  auto &window = getContext().window_;
  window.setView(view_);
  window.draw(world_);
  window.setView(window.getDefaultView());
  window.draw(container_);
}

void GameState::update(sf::Time dt) { world_.update(dt); }

#define COMMA ,
#define HANDLE_ACTIONS(NAME, PARAMS_DECL, PARAMS)                              \
  bool GameState::NAME(PARAMS_DECL) { return true; }

namespace {
sf::Vector2i normalizeVector(sf::Vector2i v, std::size_t entity,
                             sf::Vector2f offset) {
  auto vf = sf::Vector2f(v);
  vf += offset;
  return sf::Vector2i(vf.x / entity, vf.y / entity);
}
} // namespace

bool GameState::handleMouseButtonClicked(sf::Mouse::Button button,
                                         sf::Vector2i position) {
  if (container_.isInside(position)) {
    return container_.handleMouseButtonClicked(button, position);
  }
  world_.handleLeftClick(
      normalizeVector(position, ethalonDistance, view_.getOffset()));
  return false;
}
HANDLE_ACTIONS(handleMouseButtonPressed,
               sf::Mouse::Button button COMMA sf::Vector2i position,
               button COMMA position)
HANDLE_ACTIONS(handleMouseButtonReleased,
               sf::Mouse::Button button COMMA sf::Vector2i position,
               button COMMA position)
HANDLE_ACTIONS(handleMouseMoved, sf::Vector2i position, position)
HANDLE_ACTIONS(handleMouseWheelScrolled, int delta COMMA sf::Vector2i position,
               delta COMMA position)

HANDLE_ACTIONS(handleKeyClicked, sf::Keyboard::Key key, key)
bool GameState::handleKeyPressed(sf::Keyboard::Key key) {
  static float dmove = 5.f;
  switch (key) {
  case sf::Keyboard::D:
    view_.move(sf::Vector2f(dmove, 0));
    return false;
  case sf::Keyboard::W:
    view_.move(sf::Vector2f(0, -dmove));
    return false;
  case sf::Keyboard::A:
    view_.move(sf::Vector2f(-dmove, 0));
    return false;
  case sf::Keyboard::S:
    view_.move(sf::Vector2f(0, dmove));
    return false;
  }
  return true;
}
HANDLE_ACTIONS(handleKeyReleased, sf::Keyboard::Key key, key)

#undef HANDLE_ACTIONS
#undef COMMA
} // namespace heroes
