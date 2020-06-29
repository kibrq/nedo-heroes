#include "World.h"
#include "Hero.h"
#include "LandscapeObject.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>

namespace heroes {

#define BEGIN_TEXTURES(TYPE) TYPE##Textures World::TYPE##TexturesHolder_;
#include "Textures.inc"
#undef BEGIN_TEXTURES

Tile::Tile(sf::Vector2i position, const sf::Texture &texture)
    : position_{position}, sprite_{texture} {}

void Tile::drawCurrent(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(sprite_, states);
}

World::World(int width, int height) : World(sf::Vector2i(width, height)) {}
World::World(sf::Vector2i size) : size_{size} {
  loadTextures();
  buildScene();
  buildGraph();
}

void World::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(root_, states);
}
void World::update(sf::Time dt) { root_.update(dt); }
void World::handleEvent(sf::Event &event, sf::Vector2i absPos) {
  static bool mouseButtonPressed = false;
  if (event.type != sf::Event::MouseButtonPressed &&
      event.type != sf::Event::MouseButtonReleased)
    return;

  if ((mouseButtonPressed && event.type == sf::Event::MouseButtonPressed) ||
      (!mouseButtonPressed && event.type == sf::Event::MouseButtonReleased))
    return;

  mouseButtonPressed = !mouseButtonPressed;

  if (mouseButtonPressed)
    return;

  auto pos = determineTile(static_cast<float>(event.mouseButton.x - absPos.x),
                           static_cast<float>(event.mouseButton.y - absPos.y));

  bool moved = false;
  if (path_) {
    if (auto dest = path_->getDestination(); dest.has_value() && *dest == pos) {
      moved = true;
      auto anim =
          std::make_unique<HeroMovingAnimation>(std::move(path_), hero_);
      path_ = nullptr;
      root_.attachChild(std::move(anim));
    }
  }

  if (moved)
    return;

  if (!path_) {
    path_ = std::make_unique<Path>();
  } else {
    path_->clear();
  }

  auto sceneNodes = path_->build(getShortestPath(hero_->getPosition(), pos));

  for (auto &pnode : sceneNodes) {
    sceneTiles_[toID(pnode.first)]->attachChild(std::move(pnode.second));
  }
}

#define KEYBOARD_ACTION(Key, Action)                                           \
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key)) {                         \
    Action;                                                                    \
  }

void World::handleKeyboard() {
  static float dscale = 1.005f;
  static float dmove = 0.5f;
  KEYBOARD_ACTION(Z, ({ root_.scale(sf::Vector2f(dscale, dscale)); }));
  KEYBOARD_ACTION(C, ({ root_.scale(sf::Vector2f(1 / dscale, 1 / dscale)); }));
  KEYBOARD_ACTION(Left, ({ root_.move(sf::Vector2f(-dmove, 0)); }));
  KEYBOARD_ACTION(Right, ({ root_.move(sf::Vector2f(dmove, 0)); }));
  KEYBOARD_ACTION(Down, ({ root_.move(sf::Vector2f(0, dmove)); }));
  KEYBOARD_ACTION(Up, ({ root_.move(sf::Vector2f(0, -dmove)); }));
}

#undef KEYBOARD_ACTION

sf::Vector2i World::determineTile(float x, float y) const {
  return determineTile(sf::Vector2f(x, y));
}

sf::Vector2i World::determineTile(sf::Vector2f mousePosition) const {
  mousePosition -= root_.getPosition(), mousePosition /= root_.getScale().x;
  return sf::Vector2i(mousePosition /= static_cast<float>(tileSize_));
}

void World::loadTextures() {
#define TEXTURE(TYPE, NAME)                                                    \
  TYPE##TexturesHolder_.load(textures::TYPE::NAME,                             \
                             "../img/" #TYPE "/" #NAME ".png");
#include "Textures.inc"
#undef TEXTURE
}

std::size_t World::toID(Layer layer) { return static_cast<std::size_t>(layer); }
std::size_t World::toID(sf::Vector2i vec) { return toID(vec.x, vec.y); }
std::size_t World::toID(int x, int y) { return y * mapSize_.x + x; }
sf::Vector2i World::fromID(std::size_t id) {
  return sf::Vector2i(id % mapSize_.x, id / mapSize_.x);
}

namespace {
float angleBetween(sf::Vector2i v1, sf::Vector2i v2) {
  auto vecAbs = [](sf::Vector2i v) { return std::sqrt(v.x * v.x + v.y * v.y); };
  float cs = ((v1.x * v2.x + v1.y * v2.y) / (vecAbs(v1) * vecAbs(v2)));
  float sn = ((v1.x * v2.y - v1.y * v2.x) / (vecAbs(v1) * vecAbs(v2)));
  if (cs >= 0 && sn >= 0) {
    return std::acos(cs);
  } else if (cs >= 0 && sn < 0) {
    return std::asin(sn);
  } else if (cs < 0 && sn >= 0) {
    return std::acos(cs);
  } else {
    return -std::acos(cs);
  }
}

float angleBetweenNorm(sf::Vector2i v) {
  return angleBetween(sf::Vector2i(0, -1), v);
}

textures::Arrow getIdentifier(sf::Vector2i v1, sf::Vector2i v2) {
  auto approx = [](float ethalon, float val) {
    static float err = 0.5;
    return ethalon - err <= val && val <= ethalon + err;
  };
  float angle = angleBetween(v1, v2) * 180 / M_PI;
  bool left = angle >= 0;
  angle = std::abs(angle);
  if (approx(0, angle) || approx(180, angle)) {
    return textures::Arrow::Straight;
  } else if (approx(45, angle)) {
    return left ? textures::Arrow::Turn45Left : textures::Arrow::Turn45Right;
  } else if (approx(90, angle)) {
    return left ? textures::Arrow::Turn90Left : textures::Arrow::Turn90Right;
  } else if (approx(135, angle)) {
    return left ? textures::Arrow::Turn135Left : textures::Arrow::Turn135Right;
  } else {
    assert(false && "unreacheable");
  }
}

} // namespace

// Path implementation
World::Path::Edge::Edge(std::optional<sf::Vector2i> previous,
                        sf::Vector2i current,
                        std::optional<sf::Vector2i> further) {
  std::optional<sf::Vector2i> toPrevious, toNext;
  if (previous) {
    toPrevious.emplace(previous.value() - current);
  }
  if (further) {
    toNext.emplace(further.value() - current);
  }
  initTexture(toPrevious, toNext);
  if (sprite_)
    initTransforms(toPrevious, toNext);
}

void World::Path::Edge::initTexture(std::optional<sf::Vector2i> toPrevious,
                                    std::optional<sf::Vector2i> toNext) {
  if (!toPrevious) {
    return;
  }
  sprite_.emplace();
  if (!toNext) {
    sprite_->setTexture(ArrowTexturesHolder_.get(textures::Arrow::Stop));
    return;
  }

  sprite_->setTexture(ArrowTexturesHolder_.get(
      getIdentifier(toPrevious.value(), toNext.value())));
}

void World::Path::Edge::initTransforms(std::optional<sf::Vector2i> toPrevious,
                                       std::optional<sf::Vector2i> toNext) {
  auto bounds = sprite_->getLocalBounds();
  sprite_->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  move(sf::Vector2f(50.f, 50.f));
  if (!toNext) {
    return;
  }
  toPrevious.value() *= -1;
  rotate(angleBetweenNorm(toPrevious.value()) * 180 / M_PI);
}

void World::Path::Edge::drawCurrent(sf::RenderTarget &target,
                                    sf::RenderStates states) const {
  if (sprite_)
    target.draw(*sprite_, states);
}

void World::Path::clear() {
  for (auto edge : edges_) {
    edge->detachFromParent();
  }
  edges_.clear();
  route_.clear();
}

std::vector<std::pair<sf::Vector2i, std::unique_ptr<SceneNode>>>
World::Path::build(const std::vector<sf::Vector2i> &sPath) {
  std::vector<std::pair<sf::Vector2i, std::unique_ptr<SceneNode>>> nodes;
  for (std::size_t i = 0, size = sPath.size(); i < size; ++i) {
    std::optional<sf::Vector2i> previous, further;
    if (i != 0)
      previous.emplace(sPath[i - 1]);
    if (i != size - 1)
      further.emplace(sPath[i + 1]);
    auto edge = std::make_unique<Edge>(previous, sPath[i], further);
    edges_.push_back(edge.get());
    route_.push_back(sPath[i]);
    nodes.emplace_back(sPath[i], std::move(edge));
  }
  return nodes;
}

std::optional<sf::Vector2i> World::Path::getDestination() const {
  return route_.empty() ? std::optional<sf::Vector2i>{} : route_.back();
}

std::pair<sf::Vector2i, SceneNode *> World::Path::front() const {
  return {route_.front(), edges_.front()};
}

void World::Path::pop() {
  route_.pop_front();
  edges_.front()->detachFromParent();
  edges_.pop_front();
}

bool World::Path::empty() const { return edges_.empty(); }

World::HeroMovingAnimation::HeroMovingAnimation(std::unique_ptr<Path> path,
                                                Hero *hero)
    : path_{std::move(path)}, hero_{hero} {}

void World::HeroMovingAnimation::updateCurrent(sf::Time dt) {
  stored_ += dt;
  if (stored_.asMilliseconds() < 5000) {
    return;
  }
  stored_ = sf::seconds(0.f);
  if (path_->empty()) {
    detachFromParent();
    return;
  }
  sf::Vector2i nextPosition = path_->front().first;
  SceneNode *tile = path_->front().second->getParent();
  path_->pop();
  tile->attachChild(hero_->detachFromParent());
  hero_->setPosition(nextPosition);
}

std::vector<sf::Vector2i> World::getShortestPath(sf::Vector2i start,
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
  std::vector<sf::Vector2i> answer;
  for (int cur = endID; cur != -1; cur = ancestor[cur]) {
    answer.push_back(fromID(cur));
  }
  std::reverse(answer.begin(), answer.end());
  return answer;
}

void World::buildScene() {
  for (std::size_t i = 0; i < static_cast<std::size_t>(Layer::Count); ++i) {
    std::unique_ptr<SceneNode> layerPtr = std::make_unique<SceneNode>();
    layers_.push_back(layerPtr.get());
    root_.attachChild(std::move(layerPtr));
  }
  for (int y = 0; y < mapSize_.y; ++y) {
    for (int x = 0; x < mapSize_.x; ++x) {
      auto btile = std::make_unique<Tile>(
          sf::Vector2i(x, y),
          BackgroundTexturesHolder_.get(rand() % 2 == 0
                                            ? textures::Background::Grass1
                                            : textures::Background::Grass));
      auto stile = std::make_unique<SceneNode>();
      if (x != 0) {
        btile->setPosition(sf::Vector2f(tileSize_, 0)),
            stile->setPosition(sf::Vector2f(tileSize_, 0));
      } else {
        btile->setPosition(sf::Vector2f(0, tileSize_ * y)),
            stile->setPosition(sf::Vector2f(0, tileSize_ * y));
      }
      auto cur = std::make_pair(btile.get(), stile.get());
      if (x == 0) {
        layers_[toID(Layer::Background)]->attachChild(std::move(btile));
        layers_[toID(Layer::Surface)]->attachChild(std::move(stile));
      } else {
        tiles_.back()->attachChild(std::move(btile));
        sceneTiles_.back()->attachChild(std::move(stile));
      }
      tiles_.push_back(cur.first);
      sceneTiles_.push_back(cur.second);
    }
  }
  auto wood = std::make_unique<ResourceGainer>(
      sf::Vector2i(8, 5), LandscapeObjectsTexturesHolder_.get(
                              textures::LandscapeObjects::WoodGainer));
  sceneTiles_[toID(sf::Vector2i(8, 5))]->attachChild(std::move(wood));
  auto uniqHero = std::make_unique<Hero>(
      sf::Vector2i(1, 3), HeroTexturesHolder_.get(textures::Hero::HeroOnHorse));
  hero_ = uniqHero.get();
  sceneTiles_[toID(sf::Vector2i(1, 3))]->attachChild(std::move(uniqHero));
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

} // namespace heroes
