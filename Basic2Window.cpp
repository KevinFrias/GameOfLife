#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

int n = 140, m = 140;
int MAXN = 350, MAXM = 350;
int sizeCelda_X, sizeCelda_Y;

vector < vector <char>> matrix(MAXN, vector(MAXM, 'a'));
vector < vector <char>> matrix_clean(MAXN, vector(MAXM, 'a'));
vector < vector <char>> matrix_next_gen(MAXN, vector(MAXM, 'a'));


void prueba(sf::RenderTexture& innerTexture, sf::RectangleShape& celda, mutex &mutex,
            int x1, int x2, int y1, int y2, int iteracion, bool nulo){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (matrix[i][j] == 'a'){
                celda.setPosition(i*sizeCelda_X, j*sizeCelda_Y);
                celda.setFillColor(sf::Color(255,191,0));
                lock_guard<std::mutex> lock(mutex);
                innerTexture.draw(celda);
                lock_guard<std::mutex> unlock(mutex);
            }
        }
    }

    return;
}



int main() {

    // Glider
	matrix[0][1] = 'b';
	matrix[1][2] = 'b';
	matrix[2][0] = 'b';
	matrix[2][1] = 'b';
	matrix[2][2] = 'b';


    // Creamos dos ventanas, la primera para colocar el juego y la segunda para colocar la primera ventana y los botones para las opciones
    sf::RenderWindow outerWindow(sf::VideoMode(1650, 860), "Conway's Game of Life");
    sf::RenderTexture innerTexture;
    innerTexture.create(1400, 700);

    // Calculamos la altura y la anchura de las celdas dependiendo de cuantas celdas se quieran ver
    sizeCelda_X = innerTexture.getSize().x / m; sizeCelda_Y = innerTexture.getSize().y / n;
    // Creamos la figura con las medidades anteriores
    sf :: RectangleShape celda(sf::Vector2f(sizeCelda_X, sizeCelda_Y));

    // Usamos la clase Sprite para poder dibujar todas las celdas dentro de la pantalla del juego
    sf::Sprite innerSprite(innerTexture.getTexture());

    // Calculamos la posicion para poder colocar correctamente la pantalla
    int distancia_X = (outerWindow.getSize().x - innerTexture.getSize().x) - 5;
    int distancia_Y = ((outerWindow.getSize().y - innerTexture.getSize().y) / 2);

    // Posicionamos la ventana del juego
    innerSprite.setPosition(sf::Vector2f(distancia_X, distancia_Y));

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  BOTONES
    // Primero creamos la forma que van a tener los diferentes botones
    sf::RectangleShape button(sf::Vector2f(100.0f, 50.0f));
    button.setFillColor(sf::Color(77,0, 255));

    // Definimos la fuente que vamos a ocupar dentro de la ventana
    sf::Font font;
    font.loadFromFile("arial.ttf");

    // Creamos el texto para cada uno de los botones
    sf::Text buttonText("Prueba", font, 22);
    buttonText.setFillColor(sf::Color::White);

    // Posicionamos el boton y su texto correspondiente
    button.setPosition(70.0f, 225.0f);
    buttonText.setPosition(85.0f, 235.0f);
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Creamos un mutex para que al momento que ocupamos los diferentes hilos, no se pierda la informacion de las celdas que estamos dibujando
    mutex bloqueo;

    // Bucle que ocupamos para la pantalla mientras ésta esté presente
    while (outerWindow.isOpen()) {
        sf::Event event;

        // Checamos si es que en algun momento se tuvo algun evento
        while (outerWindow.pollEvent(event)) {
            // En caso de que el evento sea cerrar la ventana, cerramos completamente toda la ventana
            if (event.type == sf::Event::Closed){
                outerWindow.close();
            }
        }

        // Para la ventana donde presentamos el juego, ponemos un fondo de color
        innerTexture.clear(sf::Color::Black);

        // Limpiamos la pantalla principal y le colocamos un color de fondo
        outerWindow.clear(sf::Color(92,117,140));

        // Funcion de ayuda para calcular y dibujar las celdas del juego
        //prueba(innerTexture, celda, bloqueo, 0 , MAXN - 1, 0, MAXM - 1, 0, true);

        // Primero mostramos la pantalla del juego para que no se tenga algun efecto de parpadeo
        innerTexture.display();

        // Colocamos en la ventana principal, la ventana del juego
        outerWindow.draw(innerSprite);
        
        // Colocamos los diferentes botones
        outerWindow.draw(button);
        outerWindow.draw(buttonText);

        // Mostramos la pantalla principal con todos los elementos que colocamos anteriormente
        outerWindow.display();
    }

    return 0;
}

