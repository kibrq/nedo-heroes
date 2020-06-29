#include "LandscapeObject.h"

namespace heroes {

LandscapeObject::LandscapeObject(sf::Vector2i entry, const sf::Texture &texutre)
    : entry_{entry}, sprite_{texutre} {}

ResourceGainer::ResourceGainer(sf::Vector2i entry, const sf::Texture &texture)
    : LandscapeObject(entry, texture) {}

void ResourceGainer::heroEntered() {}
void ResourceGainer::drawCurrent(sf::RenderTarget &target,
                                 sf::RenderStates states) const {
  target.draw(sprite_, states);
}; // namespace heroes

} // namespace heroes
