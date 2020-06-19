#include "World.h"
#include "Hero.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>

namespace heroes {

BackgroundTextures World::BackgroundTexturesHolder_;
HeroTextures World::HeroTexturesHolder_;
ArrowTextures World::ArrowTexturesHolder_;

Tile::Tile(sf::Vector2i position, const sf::Texture &texture)
    : position_{position}, sprite_{texture} {}

void Tile::drawCurrent(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  target.draw(sprite_, states);
}

World::World(sf::RenderWindow &window, sf::IntRect rect)
    : window_{window}, bounds_{rect} {
  loadTextures();
  buildScene();
  buildGraph();
}

void World::draw() const { window_.draw(root_); }
void World::update(sf::Time dt) { root_.update(dt); }
void World::handleEvent(sf::Event &event) {
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

  auto pos = determineTile(event.mouseButton.x, event.mouseButton.y);
  bool moved = false;
  if (auto dest = path_.getDestination(); dest.has_value() && *dest == pos) {
    sceneTiles_[toID(pos)]->attachChild(hero_->detachFromParent());
    hero_->setPosition(pos);
    moved = true;
  }
  path_.clear();

  if (moved)
    return;

  auto sceneNodes = path_.build(getShortestPath(hero_->getPosition(), pos));

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

sf::Vector2i World::determineTile(int x, int y) const {
  return determineTile(sf::Vector2i(x, y));
}

sf::Vector2i World::determineTile(sf::Vector2i mousePosition) const {
  return mousePosition /= tileSize_;
}

void World::loadTextures() {
#define TEXTURE(TYPE, NAME)                                                    \
  TYPE##Textures##Holder_.load(textures::TYPE::NAME,                           \
                               "../img/" #TYPE "/" #NAME ".png");
#include "Textures.inc"
#undef TEXTURE
  /*
  backgroundTexturesH_.load(textures::Background::Grass, "../img/grass.jpg");
  heroTexturesH_.load(textures::Hero::HeroOnHorse, "../img/hero_on_horse.png");
  arrowImageH_.load(textures::Arrow::Straight,
                    "../img/arrow/straight_arrow.png");
  arrowImageH_.load(textures::Arrow::Stop, "../img/arrow/stop.png");
  arrowImageH_.load(textures::Arrow::Turn135Left,
                    "../img/arrow/turn45_left_arrow.png");
  arrowImageH_.load(textures::Arrow::Turn135Right,
                    "../img/arrow/turn45_right_arrow.png");
  arrowImageH_.load(textures::Arrow::Turn90Left,
                    "../img/arrow/turn90_left_arrow.png");
  arrowImageH_.load(textures::Arrow::Turn90Right,
                    "../img/arrow/turn90_right_arrow.png");
                    */
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
  dest_ = std::nullopt;
}

std::vector<std::pair<sf::Vector2i, std::unique_ptr<SceneNode>>>
World::Path::build(std::vector<sf::Vector2i> sPath) {
  std::vector<std::pair<sf::Vector2i, std::unique_ptr<SceneNode>>> nodes;
  for (std::size_t i = 0, size = sPath.size(); i < size; ++i) {
    std::optional<sf::Vector2i> previous, further;
    if (i != 0)
      previous.emplace(sPath[i - 1]);
    if (i != size - 1)
      further.emplace(sPath[i + 1]);
    auto edge = std::make_unique<Edge>(previous, sPath[i], further);
    edges_.push_back(edge.get());
    nodes.emplace_back(sPath[i], std::move(edge));
  }
  dest_ = sPath.back();
  return nodes;
}

std::optional<sf::Vector2i> World::Path::getDestination() const {
  return dest_;
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
          BackgroundTexturesHolder_.get(textures::Background::Grass));
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
