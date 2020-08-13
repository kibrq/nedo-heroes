#include "Application.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(1500, 750), "Heroes");
  heroes::Application(window).play();
}
