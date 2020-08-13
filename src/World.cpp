#include "World.h"
#include "GameState.h"
#include "Hero.h"
#include "Logger.hpp"
#include "Player.h"
#include "Textures.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>

namespace heroes {

World::Tile::Tile(const sf::Texture &texture, sf::Vector2i location,
                  World &world)
    : sprite_{texture}, Entity(location, world) {}

void World::Tile::drawCurrent(sf::RenderTarget &target,
                              sf::RenderStates states) const {
  target.draw(sprite_, states);
}

const Visitable *World::Tile::getVisitable() const { return visitable_; }
void World::Tile::setVisitable(Visitable *visitable) { visitable_ = visitable; }

void World::Tile::heroVisited(Hero *hero) {
  if (visitable_) {
    visitable_->heroVisited(hero);
  }
}

sf::Vector2f World::SceneTile::getCenter() const {
  return sf::Vector2f(ethalonDistance / 2.f, ethalonDistance / 2.f);
}

World::World(GameView &view, GameState &gameState)
    : StateRequestable(gameState), view_{view} {
  buildScene();
  buildGraph();
}

World::~World() = default;

std::size_t World::fromLayer(Layer layer) {
  return static_cast<std::size_t>(layer);
}
std::size_t World::toID(sf::Vector2i vec) { return toID(vec.x, vec.y); }
std::size_t World::toID(int x, int y) { return y * mapSize_.x + x; }
sf::Vector2i World::fromID(std::size_t id) {
  return sf::Vector2i(id % mapSize_.x, id / mapSize_.x);
}

std::deque<sf::Vector2i> World::getShortestPath(sf::Vector2i start,
                                                sf::Vector2i target) {
  std::size_t startID = toID(start), endID = toID(target);
  std::queue<std::size_t> _queue;
  std::vector<int> ancestor(mapSize_.x * mapSize_.y);
  std::vector<bool> used(mapSize_.x * mapSize_.y);
  _queue.push(startID);
  ancestor[startID] = -1;
  used[startID] = 1;
  while (!_queue.empty()) {
    std::size_t v = _queue.front();
    if (v == endID) {
      break;
    }
    _queue.pop();
    for (const auto &u : mapGraph_[v]) {
      if (!used[u]) {
        used[u] = 1;
        ancestor[u] = v;
        _queue.push(u);
      }
    }
  }
  std::deque<sf::Vector2i> answer;
  for (int cur = endID; cur != -1; cur = ancestor[cur]) {
    answer.push_back(fromID(cur));
  }
  std::reverse(answer.begin(), answer.end());
  return answer;
}

void World::buildScene() {
  for (std::size_t i = 0, size = fromLayer(Layer::Count); i < size; ++i) {
    std::unique_ptr<SceneNode> layerPtr = std::make_unique<SceneNode>();
    layers_.push_back(layerPtr.get());
    attachChild(std::move(layerPtr));
  }
  for (int y = 0; y < mapSize_.y; ++y) {
    for (int x = 0; x < mapSize_.x; ++x) {
      auto btile =
          std::make_unique<Tile>(textures::BackgroundResourcesHolder().get(
                                     textures::BackgroundKinds::Grass1),
                                 sf::Vector2i(x, y), *this);
      auto stile = std::make_unique<SceneTile>();
      if (x != 0) {
        btile->setPosition(sf::Vector2f(ethalonDistance, 0)),
            stile->setPosition(sf::Vector2f(ethalonDistance, 0));
      } else {
        btile->setPosition(sf::Vector2f(0, y * ethalonDistance)),
            stile->setPosition(sf::Vector2f(0, y * ethalonDistance));
      }
      auto cur = std::make_pair(btile.get(), stile.get());
      if (x == 0) {
        layers_[fromLayer(Layer::Background)]->attachChild(std::move(btile));
        layers_[fromLayer(Layer::Surface)]->attachChild(std::move(stile));
      } else {
        tiles_.back()->attachChild(std::move(btile));
        sceneTiles_.back()->attachChild(std::move(stile));
      }
      tiles_.push_back(cur.first);
      sceneTiles_.push_back(cur.second);
    }
  }

  auto player1 = std::make_unique<Player>(PlayerKinds::Red);

  auto hero1 = std::make_unique<Hero>(HeroKinds::Cowboy, player1->getKind(),
                                      sf::Vector2i(1, 3), *this);
  auto heroPtr1 = hero1.get();
  player1->addHero(std::move(heroPtr1));
  sceneTiles_[toID(1, 3)]->attachChild(std::move(hero1));

  auto player2 = std::make_unique<Player>(PlayerKinds::Blue);
  auto hero2 = std::make_unique<Hero>(HeroKinds::Cowboy, player2->getKind(),
                                      sf::Vector2i(5, 1), *this);
  auto heroPtr2 = hero2.get();
  player2->addHero(std::move(heroPtr2));
  sceneTiles_[toID(5, 1)]->attachChild(std::move(hero2));

  players_.push_back(std::move(player1));
  players_.push_back(std::move(player2));

  auto town = std::make_unique<Town>(TownKinds::Castle, PlayerKinds::Red,
                                     sf::Vector2i(1, 1), *this);
  sceneTiles_[toID(1, 1)]->attachChild(std::move(town));

  choosenHero_ = heroPtr1;
}

void World::buildGraph() {
  auto isInRange = [](auto left, auto right, auto val) {
    return left <= val && val < right;
  };
  std::size_t id = 0;
  mapGraph_.resize(mapSize_.x * mapSize_.y);
  for (int y = 0; y < mapSize_.y; ++y) {
    for (int x = 0; x < mapSize_.x; ++x, ++id) {
      for (int dy = 1; dy >= -1; --dy) {
        for (int dx = -1; dx <= 1; ++dx) {
          if (isInRange(0, mapSize_.x, x + dx) &&
              isInRange(0, mapSize_.y, y + dy)) {
            mapGraph_[id].push_back(toID(x + dx, y + dy));
          }
        }
      }
    }
  }
}

void World::addNode(std::unique_ptr<SceneNode> node, sf::Vector2i position) {
  struct Adder : TypeSpecifiedCommand<World> {
    Adder(std::unique_ptr<SceneNode> node, sf::Vector2i position)
        : node_{std::move(node)}, position_{position} {}
    std::unique_ptr<SceneNode> node_;
    sf::Vector2i position_;
    void typeSpecifiedAction(World *world, sf::Time dt) {
      world->forceAddNode(std::move(node_), position_);
    }
  };
  commands_.push(std::make_unique<Adder>(std::move(node), position));
}

void World::forceAddNode(std::unique_ptr<SceneNode> node,
                         sf::Vector2i position) {
  sceneTiles_[toID(position)]->attachChild(std::move(node));
}

void World::removeNode(const SceneNode *node, sf::Vector2i position) {

  commands_.push(std::make_unique<CopyableTypeSpecifiedCommand<World>>(
      [node = node, position = position](World *world, sf::Time dt) {
        world->forceRemoveNode(node, position);
      }));
}

std::unique_ptr<SceneNode> World::forceRemoveNode(const SceneNode *node,
                                                  sf::Vector2i position) {
  return sceneTiles_[toID(position)]->detachChild(node);
}

CommandQueue &World::getCommandQueue() { return commands_; }

void World::updateCurrent(sf::Time dt) {
  while (!commands_.isEmpty()) {
    onCommand(commands_.front(), dt);
    commands_.pop();
  }
}

void World::moveHero(Hero *hero, sf::Vector2i position) {
  commands_.push(std::make_unique<CopyableTypeSpecifiedCommand<World>>(
      [this, hero = hero, position = position](World *world, sf::Time dt) {
        world->forceAddNode(world->forceRemoveNode(hero, hero->getLocation()),
                            position);
        hero->setLocation(position);
      }));
}

void World::nextTurn() {
  if (choosenHero_) {
    choosenHero_->toggleHide();
  }
  ++currentPlayerNum_, currentPlayerNum_ %= players_.size();
  choosenHero_ = players_[currentPlayerNum_]->getFirstHeroOrNull();
  requestStackPush(States::BetweenTurns);
  if (choosenHero_) {
    setFocus(choosenHero_->getLocation());
    choosenHero_->toggleHide();
  }
}

void World::handleLeftClick(sf::Vector2i position) {
  if (!choosenHero_) {
    return;
  }
  auto destination = choosenHero_->backOfPath();

  if (destination.has_value() && position == *destination) {
    choosenHero_->startMoving();
  } else {
    choosenHero_->setPath(
        getShortestPath(choosenHero_->getLocation(), position));
  }
}

void World::setFocus(sf::Vector2i location) {
  view_.setCenter(sf::Vector2f(location * ethalonDistance));
}

void World::moveFocus(sf::Vector2f delta) { view_.move(delta); }

GameView::GameView(sf::Vector2i targetDimension, sf::FloatRect viewport)
    : offset_{0, 0} {
  float width = targetDimension.x * viewport.width;
  float height = targetDimension.y * viewport.height;
  reset(sf::FloatRect(0, 0, width, height));
  setViewport(viewport);
}

sf::Vector2f GameView::getOffset() const { return offset_; }

void GameView::setCenter(sf::Vector2f center) {
  sf::View::setCenter(center);
  offset_ = center - sf::View::getSize() / 2.f;
}

void GameView::move(sf::Vector2f delta) {
  sf::View::move(delta);
  offset_ += delta;
}

} // namespace heroes
