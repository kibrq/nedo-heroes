#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <vector>

namespace heroes {

enum class States { Game, BetweenTurns, Town };

class StateStack;

class State {
public:
  struct Context {
    Context(sf::RenderWindow &window) : window_{window} {}
    sf::RenderWindow &window_;
  };

public:
  State(StateStack &stack, Context context);
  virtual ~State();

  virtual void draw() = 0;
  virtual void update(sf::Time dt) = 0;

  virtual bool handleMouseButtonClicked(sf::Mouse::Button, sf::Vector2i) = 0;
  virtual bool handleMouseButtonPressed(sf::Mouse::Button, sf::Vector2i) = 0;
  virtual bool handleMouseButtonReleased(sf::Mouse::Button, sf::Vector2i) = 0;

  virtual bool handleMouseMoved(sf::Vector2i) = 0;
  virtual bool handleMouseWheelScrolled(int delta, sf::Vector2i) = 0;

  virtual bool handleKeyClicked(sf::Keyboard::Key) = 0;
  virtual bool handleKeyPressed(sf::Keyboard::Key) = 0;
  virtual bool handleKeyReleased(sf::Keyboard::Key) = 0;

public:
  void requestStackPush(States stateID);
  void requestStackPop();
  void requestStateClear();

protected:
  Context getContext() const;

private:
  StateStack &stack_;
  Context context_;
};

class StateStack : private sf::NonCopyable {
public:
  enum Action { Push, Pop, Clear };

public:
  StateStack(State::Context context);
  template <typename T> void registerState(States stateID) {
    factories_[stateID] = [this]() {
      return std::make_unique<T>(*this, context_);
    };
  }

  void draw();
  void update(sf::Time dt);

  void handleMouseButtonClicked(sf::Mouse::Button, sf::Vector2i);
  void handleMouseButtonPressed(sf::Mouse::Button, sf::Vector2i);
  void handleMouseButtonReleased(sf::Mouse::Button, sf::Vector2i);

  void handleMouseMoved(sf::Vector2i);
  void handleMouseWheelScrolled(int delta, sf::Vector2i);

  void handleKeyClicked(sf::Keyboard::Key);
  void handleKeyPressed(sf::Keyboard::Key);
  void handleKeyReleased(sf::Keyboard::Key);

  void pushState(States stateID);
  void popState();
  void clearStates();

  bool isEmpty() const;

private:
  std::unique_ptr<State> createState(States stateID);
  void applyPendingChanges();

private:
  struct PendingChange {
    Action action;
    States stateID;
  };

private:
  std::vector<std::unique_ptr<State>> stack_;
  std::vector<PendingChange> pendingList_;
  State::Context context_;
  std::map<States, std::function<std::unique_ptr<State>()>> factories_;
};

} // namespace heroes
