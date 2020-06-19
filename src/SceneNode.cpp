#include "SceneNode.h"
#include <cassert>
#include <iostream>

namespace heroes {

void SceneNode::attachChild(std::unique_ptr<SceneNode> node) {
  node->parent_ = this;
  children_.push_back(std::move(node));
}

std::unique_ptr<SceneNode> SceneNode::detachChild(const SceneNode &node) {
  auto iter = std::find_if(
      children_.begin(), children_.end(),
      [&node](const auto &nodePtr) { return nodePtr.get() == &node; });
  std::unique_ptr<SceneNode> detached = std::move(*iter);
  detached->parent_ = nullptr;
  children_.erase(iter);
  return std::move(detached);
}

std::unique_ptr<SceneNode> SceneNode::detachFromParent() {
  return parent_->detachChild(*this);
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  drawCurrent(target, states);
  for (const auto &ch : children_) {
    ch->draw(target, states);
  }
}

void SceneNode::update(sf::Time dt) {
  updateCurrent(dt);
  for (auto &ch : children_) {
    ch->update(dt);
  }
}

void SceneNode::updateCurrent(sf::Time dt) {}

void SceneNode::drawCurrent(sf::RenderTarget &target,
                            sf::RenderStates states) const {}

} // namespace heroes
