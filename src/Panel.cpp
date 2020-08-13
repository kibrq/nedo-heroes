#include "GUI.h"

namespace heroes::GUI {

Panel::Panel() : Component() {}
Panel::Panel(sf::Vector2i size) : Component{size} {}

void Panel::pack(std::shared_ptr<Component> component) {
  component->setParent(this);
  children_.push_back(component);
}

void Panel::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  for (const auto &comp : children_) {
    target.draw(*comp, states);
  }
}

#define COMMA ,
#define HANDLE_ACTIONS(NAME, PARAM_DECL, PARAM)                                \
  bool Panel::NAME(PARAM_DECL) {                                               \
    for (auto &comp : children_) {                                             \
      if (!comp->NAME(PARAM)) {                                                \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
    return true;                                                               \
  }

HANDLE_ACTIONS(handleMouseButtonClicked,
               sf::Mouse::Button button COMMA sf::Vector2i position,
               button COMMA position)
HANDLE_ACTIONS(handleMouseButtonPressed,
               sf::Mouse::Button button COMMA sf::Vector2i position,
               button COMMA position)
HANDLE_ACTIONS(handleMouseButtonReleased,
               sf::Mouse::Button button COMMA sf::Vector2i position,
               button COMMA position)

HANDLE_ACTIONS(handleKeyClicked, sf::Keyboard::Key key, key)
HANDLE_ACTIONS(handleKeyPressed, sf::Keyboard::Key key, key)
HANDLE_ACTIONS(handleKeyReleased, sf::Keyboard::Key key, key)

HANDLE_ACTIONS(handleMouseMoved, sf::Vector2i position, position)
HANDLE_ACTIONS(handleMouseWheelScrolled, int delta COMMA sf::Vector2i position,
               delta COMMA position)

} // namespace heroes::GUI
