#include "Logger.hpp"
#include <algorithm>
#include <cassert>

namespace heroes {

template <typename T>
void TreeLikeDrawable<T>::attachChild(std::unique_ptr<T> node) {
  node->parent_ = static_cast<T *>(this);
  children_.push_back(std::move(node));
}

template <typename T>
std::unique_ptr<T> TreeLikeDrawable<T>::detachChild(const T *node) {
  auto iter = std::find_if(children_.begin(), children_.end(),
                           [node](auto &child) { return child.get() == node; });
  std::unique_ptr<T> result{std::move(*iter)};
  children_.erase(iter);
  return result;
}

template <typename T>
std::unique_ptr<T> TreeLikeDrawable<T>::detachFromParent() {
  return parent_->detachChild(static_cast<T *>(this));
}

template <typename T>
void TreeLikeDrawable<T>::draw(sf::RenderTarget &target,
                               sf::RenderStates states) const {
  states.transform *= getTransform();
  drawCurrent(target, states);
  for (const auto &ch : children_) {
    ch->draw(target, states);
  }
}

template <typename T> void TreeLikeDrawable<T>::update(sf::Time dt) {
  updateCurrent(dt);
  for (const auto &ch : children_) {
    ch->update(dt);
  }
}

template <typename T>
sf::Transform TreeLikeDrawable<T>::getAbsoluteTransform() const {
  sf::Transform tranform = sf::Transform::Identity;
  for (const T *current = static_cast<const T *>(this);
       current->parent_ != nullptr; current = current->parent_)
    tranform *= current->getTransform();
  return tranform;
}

template <typename T>
sf::Vector2i TreeLikeDrawable<T>::getAbsolutePosition() const {
  return sf::Vector2<int>(getAbsoluteTransform() * sf::Vector2f());
}
} // namespace heroes
