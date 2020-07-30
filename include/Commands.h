#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include <functional>

namespace heroes {

struct SceneNode;

struct Command {
  virtual void operator()(SceneNode *node, sf::Time dt) = 0;
  void doAction(SceneNode *node, sf::Time dt) { (*this)(node, dt); }
};

struct SimpleCommand : Command {
  SimpleCommand(std::function<void(SceneNode *node, sf::Time dt)> action)
      : action_{action} {}
  std::function<void(SceneNode *node, sf::Time dt)> action_;
  void operator()(SceneNode *node, sf::Time dt) { action_(node, dt); }
};

class CommandQueue : private sf::NonCopyable {

public:
  void push(std::unique_ptr<Command> command) {
    queue_.push_back(std::move(command));
  }
  Command *front() { return queue_.front().get(); }
  void pop() { queue_.pop_front(); }

  bool isEmpty() const { return queue_.empty(); }

private:
  std::deque<std::unique_ptr<Command>> queue_;
};

} // namespace heroes
