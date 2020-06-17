#pragma once

#include "ResourceHolder.hpp"
#include "SceneNode.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace heroes {

namespace textures {
enum class Background { Grass };
enum class Hero { HeroOnHorse };
enum class Arrow {
  Straight,
  Turn45Left,
  Turn45Right,
  Turn90Left,
  Turn90Right,
  Stop
};
} // namespace textures

using BackgroundTexturesHolder =
    ResourceHolder<textures::Background, sf::Texture>;
using HeroTexturesHolder = ResourceHolder<textures::Hero, sf::Texture>;
using ArrowImageHolder = ResourceHolder<textures::Arrow, sf::Texture>;
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
class World {
public:
  World(sf::RenderWindow &window, sf::IntRect rect);
  void draw() const;
  void update(sf::Time);
  void handleEvent(sf::Event &event);

private:
  sf::Vector2i determineTile(int x, int y) const;
  sf::Vector2i determineTile(sf::Vector2i mousePosition) const;

private:
  static BackgroundTexturesHolder backgroundTexturesH_;
  static HeroTexturesHolder heroTexturesH_;
  static ArrowImageHolder arrowImageH_;

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
    build(std::vector<sf::Vector2i> sPath);

  private:
    struct Edge : SceneNode {
      Edge(std::optional<sf::Vector2i> previous, sf::Vector2i current,
           std::optional<sf::Vector2i> further);

    private:
      void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;

    private:
      sf::Sprite sprite_;
    };
    std::vector<Edge *> edges_;
  };

  Path path_;

private:
  void buildGraph();
  std::vector<std::vector<std::size_t>> mapGraph_;
  std::vector<sf::Vector2i> getShortestPath(sf::Vector2i start,
                                            sf::Vector2i target);

private:
  sf::RenderWindow &window_;
  sf::IntRect bounds_;
  sf::Vector2i mapSize_{10, 10};
  int tileSize_{100};
};

} // namespace heroes
