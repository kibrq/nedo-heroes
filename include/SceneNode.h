#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace heroes {
struct SceneNode : public sf::Transformable,
                   public sf::Drawable,
                   private sf::NonCopyable {
  SceneNode() = default;

  void attachChild(std::unique_ptr<SceneNode>);
  std::unique_ptr<SceneNode> detachChild(const SceneNode &);
  std::unique_ptr<SceneNode> detachFromParent();

  SceneNode *getParent() const;

  void update(sf::Time dt);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
  virtual void drawCurrent(sf::RenderTarget &target,
                           sf::RenderStates states) const;
  virtual void updateCurrent(sf::Time dt);

private:
  std::vector<std::unique_ptr<SceneNode>> children_;
  SceneNode *parent_{nullptr};
};

struct SceneAnimation : SceneNode {
  SceneAnimation() = default;

private:
  void drawCurrent(sf::RenderTarget &target,
                   sf::RenderStates states) const override final {}
  using SceneNode::attachChild;
  using SceneNode::detachChild;
};
} // namespace heroes
