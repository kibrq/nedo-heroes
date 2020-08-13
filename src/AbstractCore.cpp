#include "AbstractCore.h"
#include "Logger.hpp"
#include <cassert>

namespace heroes {

SceneNode::~SceneNode() {}

void SceneNode::attachChild(std::unique_ptr<SceneNode> node) {
  node->setParent(this);
  children_.push_back(std::move(node));
}

std::unique_ptr<SceneNode> SceneNode::detachChild(const SceneNode *node) {
  auto iter = std::find_if(children_.begin(), children_.end(),
                           [node](auto &child) { return child.get() == node; });
  auto result{std::move(*iter)};
  children_.erase(iter);
  return result;
}

std::unique_ptr<SceneNode> SceneNode::detachFromParent() {
  assert(parent_);
  return parent_->detachChild(this);
}

const SceneNode *SceneNode::getParent() const { return parent_; }

void SceneNode::setParent(SceneNode *parent) {
  parent_ = parent;
  onSetParent();
}

void SceneNode::onSetParent() {}

sf::Vector2f SceneNode::getCenter() const { return sf::Vector2f(0, 0); }

void SceneNode::update(sf::Time dt) {
  updateCurrent(dt);
  for (const auto &ch : children_) {
    ch->update(dt);
  }
}

void SceneNode::updateCurrent(sf::Time dt) {}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  drawCurrent(target, states);
  for (const auto &ch : children_) {
    ch->draw(target, states);
  }
}

void SceneNode::drawCurrent(sf::RenderTarget &target,
                            sf::RenderStates states) const {}

sf::Transform SceneNode::getAbsoluteTransform() const {
  sf::Transform result = sf::Transform::Identity;
  for (const auto *current = this; current != nullptr; current = getParent()) {
    result *= current->getTransform();
  }
  return result;
}

sf::Vector2f SceneNode::getAbsolutePosition() const {
  return sf::Vector2<float>(getAbsoluteTransform() * sf::Vector2f());
}

void SceneNode::onCommand(Command *command, sf::Time dt) {
  command->doAction(this, dt);
  for (auto &child : children_) {
    child->onCommand(command, dt);
  }
}

Animation::Animation(sf::Time duration, bool repetative)
    : duration_{duration}, needRepeat_{repetative} {}

bool Animation::update(sf::Time dt) {
  auto timePerFrame = duration_ / static_cast<float>(frames_.size());
  bool result = false;
  if (isStopped) {
    return result;
  }
  elapsedTime_ += dt;
  if (elapsedTime_ >= timePerFrame) {
    elapsedTime_ -= timePerFrame;
    ++currentFrame_;
    if (currentFrame_ == frames_.size()) {
      result = true;
      currentFrame_ = 0;
      if (!needRepeat_) {
        stop();
      }
    }
  }
  return result;
}

void Animation::stop() {
  isStopped = true;
  elapsedTime_ = sf::Time::Zero;
}

void Animation::reset() {
  stop();
  currentFrame_ = 0;
}

void Animation::resume() { isStopped = false; }

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (frames_.empty()) {
    return;
  }
  target.draw(frames_[currentFrame_], states);
}

} // namespace heroes

// namespace heroes
