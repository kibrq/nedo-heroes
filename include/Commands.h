#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include <functional>

namespace heroes {

struct SceneNode;

struct Command {
  virtual bool isTarget(SceneNode *node) = 0;
  virtual void operator()(SceneNode *node, sf::Time dt) = 0;
  void doAction(SceneNode *node, sf::Time dt) {
    if (isTarget(node))
      (*this)(node, dt);
  }
};

struct CopyableCommand : Command {

  CopyableCommand(std::function<bool(SceneNode *)> isTarget,
                  std::function<void(SceneNode *, sf::Time)> action)
      : isTarget_{std::move(isTarget)}, action_{std::move(action)} {}

  bool isTarget(SceneNode *node) { return isTarget_(node); }
  void operator()(SceneNode *node, sf::Time dt) { action_(node, dt); }

private:
  std::function<bool(SceneNode *)> isTarget_;
  std::function<void(SceneNode *, sf::Time)> action_;
};

template <typename T> struct TypeSpecifiedCommand : Command {
  bool isTarget(SceneNode *node) override {
    return node && dynamic_cast<T *>(node);
  }

  void operator()(SceneNode *node, sf::Time dt) {
    typeSpecifiedAction(dynamic_cast<T *>(node), dt);
  }

  virtual void typeSpecifiedAction(T *node, sf::Time dt) = 0;
};

template <typename T>
struct CopyableTypeSpecifiedCommand : TypeSpecifiedCommand<T> {
  CopyableTypeSpecifiedCommand(std::function<void(T *node, sf::Time dt)> action)
      : action_{action} {}
  void typeSpecifiedAction(T *node, sf::Time dt) { action_(node, dt); }

private:
  std::function<void(T *node, sf::Time dt)> action_;
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
