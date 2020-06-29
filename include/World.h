#pragma once

#include "ResourceHolder.hpp"
#include "SceneNode.h"
#include "Textures.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <unordered_map>

namespace heroes {
#define BEGIN_TEXTURES(TYPE)                                                   \
  using TYPE##Textures = ResourceHolder<textures::TYPE, sf::Texture>;
#include "Textures.inc"
#undef BEGIN_TEXTURES

struct Tile : SceneNode {
  Tile(sf::Vector2i, const sf::Texture &);
  sf::Vector2i getPosition() const { return position_; }

private:
  void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;

private:
  sf::Vector2i position_;
  sf::Sprite sprite_;
};

class Hero;

class World : public sf::Drawable {
public:
  World(int width, int height);
  World(sf::Vector2i size);

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
  void update(sf::Time);
  void handleEvent(sf::Event &event, sf::Vector2i absPos);
  void handleKeyboard();

private:
  sf::Vector2i determineTile(float x, float y) const;
  sf::Vector2i determineTile(sf::Vector2f mousePosition) const;

private:
#define BEGIN_TEXTURES(TYPE) static TYPE##Textures TYPE##TexturesHolder_;
#include "Textures.inc"
#undef BEGIN_TEXTURES

private:
  void loadTextures();

private:
  enum class Layer : std::size_t { Background = 0, Surface = 1, Count = 2 };
  std::vector<SceneNode *> layers_;
  std::vector<SceneNode *> sceneTiles_;
  std::vector<Tile *> tiles_;
  std::size_t toID(Layer layer);
  std::size_t toID(sf::Vector2i vec);
  std::size_t toID(int x, int y);
  sf::Vector2i fromID(std::size_t);

private:
  SceneNode root_;
  void buildScene();

private:
  Hero *hero_;

private:
  struct Path {
    void clear();
    std::vector<std::pair<sf::Vector2i, std::unique_ptr<SceneNode>>>
    build(const std::vector<sf::Vector2i> &sPath);
    std::optional<sf::Vector2i> getDestination() const;
    std::pair<sf::Vector2i, SceneNode *> front() const;
    void pop();
    bool empty() const;

  private:
    struct Edge : SceneNode {
      Edge(std::optional<sf::Vector2i> previous, sf::Vector2i current,
           std::optional<sf::Vector2i> further);

    private:
      void initTexture(std::optional<sf::Vector2i> toPrevious,
                       std::optional<sf::Vector2i> toNext);
      void initTransforms(std::optional<sf::Vector2i> toPrevious,
                          std::optional<sf::Vector2i> toNext);
      void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;

    private:
      std::optional<sf::Sprite> sprite_;
    };
    std::deque<sf::Vector2i> route_;
    std::deque<Edge *> edges_;
  };

  std::unique_ptr<Path> path_;

private:
  struct HeroMovingAnimation : SceneAnimation {
    HeroMovingAnimation(std::unique_ptr<Path> path, Hero *hero);

  private:
    void updateCurrent(sf::Time dt) override;

  private:
    sf::Time stored_;

  private:
    std::unique_ptr<Path> path_;
    Hero *hero_;
  };

private:
  void buildGraph();
  std::vector<std::vector<std::size_t>> mapGraph_;
  std::vector<sf::Vector2i> getShortestPath(sf::Vector2i start,
                                            sf::Vector2i target);

private:
  sf::Vector2i size_;
  sf::Vector2i mapSize_{12, 6};
  int tileSize_{100};
};

} // namespace heroes
