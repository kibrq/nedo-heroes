#pragma once

#include "AbstractCore.h"
#include "GameObjectsCore.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <unordered_map>

namespace heroes {

constexpr int ethalonDistance{100};

class World;
class Player;
class Hero;
class GameState;

struct GameView : sf::View {
  GameView(sf::Vector2i windowDimension, sf::FloatRect viewport);

  void setCenter(sf::Vector2f center);
  void move(sf::Vector2f delta);

  sf::Vector2f getOffset() const;

private:
  sf::Vector2f offset_;
};

class World final : public SceneNode, public StateRequestable<GameState> {

private:
  struct Tile : Entity, Visitable {
    Tile(const sf::Texture &texture, sf::Vector2i loc, World &world);

  private:
    void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;
    sf::Sprite sprite_;

  public:
    void setVisitable(Visitable *visitable);
    const Visitable *getVisitable() const;

    void heroVisited(Hero *hero);

  private:
    Visitable *visitable_{nullptr};
  };

  struct SceneTile : SceneNode {
    SceneTile() = default;

  private:
    sf::Vector2f getCenter() const override;
  };

public:
  World(GameView &, GameState &);
  ~World();

  void setFocus(sf::Vector2i location);
  void moveFocus(sf::Vector2f delta);

private:
  GameView &view_;
  void buildScene();

  enum class Layer : std::size_t { Background = 0, Surface = 1, Count = 2 };
  std::size_t fromLayer(Layer);
  Layer toLayer(std::size_t);
  std::vector<SceneNode *> layers_;

  std::size_t toID(sf::Vector2i vec);
  std::size_t toID(int x, int y);
  sf::Vector2i fromID(std::size_t);

  std::vector<SceneTile *> sceneTiles_;
  std::vector<Tile *> tiles_;

  sf::Vector2i mapSize_{20, 20};

  void buildGraph();
  std::vector<std::vector<std::size_t>> mapGraph_;

public:
  std::deque<sf::Vector2i> getShortestPath(sf::Vector2i start,
                                           sf::Vector2i target);

private:
  void updateCurrent(sf::Time dt) override;

private:
  std::vector<std::unique_ptr<Player>> players_;
  std::size_t currentPlayerNum_{0};
  Hero *choosenHero_{nullptr};

public:
  CommandQueue &getCommandQueue();

private:
  CommandQueue commands_;

public:
  void nextTurn();
  void addNode(std::unique_ptr<SceneNode>, sf::Vector2i position);
  void removeNode(const SceneNode *, sf::Vector2i position);
  void moveHero(Hero *hero, sf::Vector2i newPosition);

private:
  void forceAddNode(std::unique_ptr<SceneNode>, sf::Vector2i position);
  std::unique_ptr<SceneNode> forceRemoveNode(const SceneNode *,
                                             sf::Vector2i position);

public:
  void handleLeftClick(sf::Vector2i position);
  void handleRightPress(sf::Vector2i position);
  void handleRightRelease(sf::Vector2i position);
};

} // namespace heroes
