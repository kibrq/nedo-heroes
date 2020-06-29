#include "Panel.h"
#include "World.h"

namespace heroes {
struct HelperGamePanel : Panel {
  HelperGamePanel(sf::Vector2i size, Panel *parent);

  void setWorld(World *world) { world_ = world; }

private:
  void drawCurrent(sf::RenderTarget &target,
                   sf::RenderStates states) const override;
  void handleEventCurrent(sf::Event &event) override;
  void handleKeyboardCurrent() override;
  void updateCurrent(sf::Time dt) override;

private:
  World *world_;
};

struct GameInfoPanel : Panel {
  GameInfoPanel(sf::Vector2i size, Panel *parent);

  void setWorld(World *world) { world_ = world; }

private:
  void drawCurrent(sf::RenderTarget &target,
                   sf::RenderStates states) const override;
  void handleEventCurrent(sf::Event &event) override;

private:
  sf::CircleShape button_;
  sf::RectangleShape background_;
  World *world_;
};

struct MainGamePanel : Panel {

private:
  static inline constexpr float ratio = 0.8;

public:
  MainGamePanel(int width, int height);
  MainGamePanel(sf::Vector2i size);

private:
  World world_;

}; // namespace heroes
} // namespace heroes
