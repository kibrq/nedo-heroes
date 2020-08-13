#include "States.h"
#include "Logger.hpp"
#include <cassert>

namespace heroes {

State::State(StateStack &stack, Context context)
    : stack_{stack}, context_{context} {}

State::~State(){};

void State::requestStackPush(States id) { stack_.pushState(id); }

void State::requestStackPop() { stack_.popState(); }

void State::requestStateClear() { stack_.clearStates(); }

auto State::getContext() const -> Context { return context_; }

StateStack::StateStack(State::Context context) : context_{context} {}

std::unique_ptr<State> StateStack::createState(States id) {
  auto result = factories_.find(id);
  assert(result != factories_.end());
  return result->second();
}

void StateStack::draw() {
  for (auto &state : stack_) {
    state->draw();
  }
  applyPendingChanges();
}

void StateStack::update(sf::Time dt) {
  for (auto &state : stack_) {
    state->update(dt);
  }
  applyPendingChanges();
}

#define COMMA ,
#define HANDLE_ACTION(NAME, PARAMS_DECL, PARAMS)                               \
  void StateStack::NAME(PARAMS_DECL) {                                         \
    for (auto iter = stack_.rbegin(); iter != stack_.rend(); ++iter) {         \
      if (!(*iter)->NAME(PARAMS)) {                                            \
        break;                                                                 \
      }                                                                        \
      applyPendingChanges();                                                   \
    }                                                                          \
  }

HANDLE_ACTION(handleMouseButtonClicked,
              sf::Mouse::Button button COMMA sf::Vector2i position,
              button COMMA position)
HANDLE_ACTION(handleMouseButtonPressed,
              sf::Mouse::Button button COMMA sf::Vector2i position,
              button COMMA position)
HANDLE_ACTION(handleMouseButtonReleased,
              sf::Mouse::Button button COMMA sf::Vector2i position,
              button COMMA position)

HANDLE_ACTION(handleMouseMoved, sf::Vector2i position, position)
HANDLE_ACTION(handleMouseWheelScrolled, int delta COMMA sf::Vector2i position,
              delta COMMA position)

HANDLE_ACTION(handleKeyClicked, sf::Keyboard::Key key, key)
HANDLE_ACTION(handleKeyPressed, sf::Keyboard::Key key, key)
HANDLE_ACTION(handleKeyReleased, sf::Keyboard::Key key, key)

void StateStack::applyPendingChanges() {
  for (const auto &change : pendingList_) {
    switch (change.action) {
    case Push:
      stack_.push_back(createState(change.stateID));
      break;
    case Pop:
      stack_.pop_back();
      break;
    case Clear:
      stack_.clear();
      break;
    }
  }
  pendingList_.clear();
}

void StateStack::pushState(States stateID) {
  pendingList_.push_back(PendingChange{Push, stateID});
}

void StateStack::popState() { pendingList_.push_back(PendingChange{Pop}); }

void StateStack::clearStates() { pendingList_.push_back(PendingChange{Clear}); }

bool StateStack::isEmpty() const { return stack_.empty(); }

} // namespace heroes
