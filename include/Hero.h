#pragma once

#include "AbstractCore.h"
#include "GameObjectsCore.h"
#include <SFML/Graphics.hpp>
#include <deque>

namespace heroes {

enum class HeroKinds { Cowboy };
enum class HeroMovingAnimationKinds { MoveEast, MoveWest };

class Hero : public OwnableEntity {
public:
public:
  Hero(HeroKinds heroKind, PlayerKinds owner, sf::Vector2i location, World &);

private:
  struct HeroMovingAnimation : Animation {
    HeroMovingAnimation(HeroKinds heroKind, HeroMovingAnimationKinds animKind,
                        sf::Time duration);
  };
  HeroMovingAnimation moveEast_, moveWest_;
  HeroMovingAnimation *currentAnimation_;

  void chooseAnimationByDirection(sf::Vector2i direction);

public:
  void setLocation(sf::Vector2i);

private:
  void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;
  void updateCurrent(sf::Time dt) override;

private:
  struct Path {
    Path(const std::deque<sf::Vector2i> &route_, World &world);
    ~Path();
    void pop();
    sf::Vector2i front() const;
    sf::Vector2i direction() const;

    struct Edge : SimpleEntity {
      Edge(std::optional<sf::Vector2i> previous, sf::Vector2i current,
           std::optional<sf::Vector2i> further);

      void initTexture(std::optional<sf::Vector2i> toPrevious,
                       std::optional<sf::Vector2i> toNext);
      void initTransforms(std::optional<sf::Vector2i> toPrevious,
                          std::optional<sf::Vector2i> toNext);
      void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;

      void toggleHide();

      sf::Vector2i getLocation() const;
      sf::Vector2i getDirection() const;

      bool hide_{false};
      std::optional<sf::Sprite> sprite_;
      sf::Vector2i direction_;
    };
    std::deque<Edge *> edges_;
    World &world_;
  };
  std::unique_ptr<Path> path_{nullptr};

public:
  std::optional<sf::Vector2i> backOfPath() const;

  bool isPathEmpty() const;

  void setPath(const std::deque<sf::Vector2i> &route);

  void startMoving();

  void stopMoving();

  void toggleHide();

private:
  bool isMoving_{false};
};

} // namespace heroes
