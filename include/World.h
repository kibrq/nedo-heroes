#pragma once

#include "AbstractCore.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <unordered_map>

namespace heroes {

constexpr int ethalonDistance{100};

class World;
class Player;
class Hero;

struct SimpleEntity : SceneNode {
  SimpleEntity(sf::Vector2i loaction);
  sf::Vector2i getLocation() const;
  void setLocation(sf::Vector2i);

protected:
  sf::Vector2i location_;
};

struct Entity : SimpleEntity {
  Entity(sf::Vector2i location, World &world);

protected:
  World &world_;
};

class World final : public SceneNode {

private:
  struct Tile : Entity {
    Tile(const sf::Texture &texture, sf::Vector2i loc, World &world);

  private:
    void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;

  private:
    sf::Sprite sprite_;
  };

public:
  World();
  ~World();

private:
  void buildScene();

  enum class Layer : std::size_t { Background = 0, Surface = 1, Count = 2 };
  std::size_t fromLayer(Layer);
  Layer toLayer(std::size_t);
  std::vector<SceneNode *> layers_;

  std::size_t toID(sf::Vector2i vec);
  std::size_t toID(int x, int y);
  sf::Vector2i fromID(std::size_t);

  std::vector<SceneNode *> sceneTiles_;
  std::vector<Tile *> tiles_;

  sf::Vector2i mapSize_{12, 6};

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
  void forceAddNode(std::unique_ptr<SceneNode>, sf::Vector2i position);
  void removeNode(const SceneNode *, sf::Vector2i position);
  std::unique_ptr<SceneNode> forceRemoveNode(const SceneNode *,
                                             sf::Vector2i position);
  void moveHero(Hero *hero, sf::Vector2i newPosition);

public:
  void handleLeftClick(sf::Vector2i position);
  void handleRightPress(sf::Vector2i position);
  void handleRightRelease(sf::Vector2i position);
};

struct WorldWrapperPanel final : public Panel {
  WorldWrapperPanel(int width, int height);
  WorldWrapperPanel(sf::Vector2i size);

private:
  void drawCurrent(sf::RenderTarget &, sf::RenderStates) const override;
  void updateCurrent(sf::Time dt) override;

private:
  bool handleMouseButtonClickedCurrent(sf::Mouse::Button,
                                       sf::Vector2i position) override;
  bool handleKeyPressedCurrent(sf::Keyboard::Key) override;
  bool handleKeyClickedCurrent(sf::Keyboard::Key) override;

private:
  sf::Vector2i normalizeVector(sf::Vector2i) const;

private:
  World world_;
};

} // namespace heroes
