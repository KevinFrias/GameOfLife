#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

int main() {

    sf::RenderWindow outerWindow(sf::VideoMode(1650, 810), "Outer Window");

    sf::RenderTexture innerTexture;
    innerTexture.create(1200, 710);

    sf::Sprite innerSprite(innerTexture.getTexture());

    int distancia_X = (outerWindow.getSize().x - innerTexture.getSize().x);
    int distancia_Y = (outerWindow.getSize().y - innerTexture.getSize().y) / 2;


    innerSprite.setPosition(sf::Vector2f(distancia_X - 40, distancia_Y));

    while (outerWindow.isOpen()) {
        sf::Event event;

        while (outerWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                outerWindow.close();
            }
        }

        innerTexture.clear(sf::Color::Black);

        // Draw stuff onto the inner window
        // ...

        innerTexture.display();

        // Asignamos un color de fondo para la pantalla del fondo, este caso, Negro
        outerWindow.clear(sf::Color(0x808080));
        // Draw the inner window onto the outer window
        outerWindow.draw(innerSprite);
        outerWindow.display();
    }

    return 0;
}

