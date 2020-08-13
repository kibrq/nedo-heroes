#pragma once

#include "Commands.h"
#include "States.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace heroes {

struct SceneNode : sf::Drawable, sf::Transformable, private sf::NonCopyable {
  SceneNode() = default;

  virtual ~SceneNode();

  void attachChild(std::unique_ptr<SceneNode>);
  std::unique_ptr<SceneNode> detachChild(const SceneNode *);
  std::unique_ptr<SceneNode> detachFromParent();

  const SceneNode *getParent() const;
  void setParent(SceneNode *parent);

  virtual sf::Vector2f getCenter() const;

protected:
  virtual void onSetParent();

public:
  void update(sf::Time dt);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
  virtual void drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const;
  virtual void updateCurrent(sf::Time dt);

protected:
  sf::Transform getAbsoluteTransform() const;
  sf::Vector2f getAbsolutePosition() const;

  void onCommand(Command *command, sf::Time dt);

protected:
  std::vector<std::unique_ptr<SceneNode>> children_;
  SceneNode *parent_{nullptr};
};

struct Animation : sf::Drawable, sf::Transformable, private sf::NonCopyable {
  Animation(sf::Time duration, bool repetative);

public:
  bool update(sf::Time dt);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
  sf::Time duration_, elapsedTime_{sf::Time::Zero};

public:
  void stop();
  void reset();
  void resume();

private:
  bool isStopped{true};
  bool needRepeat_;

protected:
  std::vector<sf::Sprite> frames_;
  std::size_t currentFrame_{0};
};

template <typename Parent> struct StateRequestable {
  StateRequestable(Parent &parent) : parent_{parent} {}

protected:
  void requestStackPush(States stateID) { parent_.requestStackPush(stateID); }
  void requestStackPop() { parent_.requestStackPop(); }
  void requestStackClear() { parent_.requestStackClear(); }

protected:
  Parent &parent_;
};

} // namespace heroes
