#include "AbstractCore.h"
#include "World.h"

namespace heroes {

struct GameInfoPanel : Panel {
  GameInfoPanel(int width, int height);
  GameInfoPanel(sf::Vector2i size);

  void setWorld(World *world) { world_ = world; }

private:
  void drawCurrent(sf::RenderTarget &target,
                   sf::RenderStates states) const override;

private:
  sf::RectangleShape background_;
  World *world_;
};

struct MainGamePanel : Panel {

private:
  static inline constexpr float ratio = 0.8;

public:
  MainGamePanel(int width, int height);
  MainGamePanel(sf::Vector2i size);

}; // namespace heroes
} // namespace heroes
