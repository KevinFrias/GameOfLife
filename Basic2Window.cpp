#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

int n = 20, m = 20;
vector < vector <char>> matrix(n, vector(m, 'a'));
vector < vector <char>> matrix_clean(n, vector(m, 'a'));
vector < vector <char>> matrix_next_gen(n, vector(m, 'a'));

int main() {

    // Glider
	matrix[0][1] = 'b';
	matrix[1][2] = 'b';
	matrix[2][0] = 'b';
	matrix[2][1] = 'b';
	matrix[2][2] = 'b';



    sf::RenderWindow outerWindow(sf::VideoMode(1650, 810), "Outer Window");

    sf::RenderTexture innerTexture;
    innerTexture.create(1200, 710);

    sf::Sprite innerSprite(innerTexture.getTexture());

    int distancia_X = (outerWindow.getSize().x - innerTexture.getSize().x) - 40;
    int distancia_Y = (outerWindow.getSize().y - innerTexture.getSize().y) / 2;

    innerSprite.setPosition(sf::Vector2f(distancia_X, distancia_Y));


    int sizeCelda_X = innerTexture.getSize().x / m , sizeCelda_Y = innerTexture.getSize().y / n;



    while (outerWindow.isOpen()) {
        sf::Event event;

        while (outerWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                outerWindow.close();
            }
        }


        // Empezamos a declarar la forma que va a tomar las celdas dentro del juego de la vida

//--- El tama;o de la celda va a cambiar conforme se tenga mas o menos zoom

        sf :: RectangleShape celda(sf::Vector2f(sizeCelda_X, sizeCelda_Y));
        celda.setOutlineColor(sf::Color::White);
        celda.setOutlineThickness(2.f);

        innerTexture.clear(sf::Color::Black);
        outerWindow.clear(sf::Color(0x808080));

// -- Aqui hay que dividir el poner todas las celdas que esten encendidas dentor del innerWindow
//-- Tenemos que hcaer udo de multithreading

        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                if (matrix[i][j] == 'b'){
                    celda.setPosition(i*sizeCelda_X, j*sizeCelda_Y);
                    celda.setFillColor(sf::Color::White);
                    innerTexture.draw(celda);
                }
                /*
                else {
                    celda.setPosition(i*sizeCelda_X, j*sizeCelda_Y);
                    celda.setFillColor(sf::Color::Black);
                    innerTexture.draw(celda);
                }
                */
            }
        }


        // Mostramos la ventana donde mostramos el juego de la vida
        innerTexture.display();

        // Colocamos la ventana donde se muestra el juego, dentro de la ventana donde 
        // se colocó todas las opciones de visualizacion
        outerWindow.draw(innerSprite);
        outerWindow.display();
    }

    return 0;
}
