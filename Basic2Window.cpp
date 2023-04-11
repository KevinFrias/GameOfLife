#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <semaphore.h>
#define PB push_back
using namespace std;

int n = 50;
int sizeCelda_X, sizeCelda_Y;

vector < vector <bool>> matrix(n, vector(n, false));
vector < vector <bool>> matrix_clean(n, vector(n, false));
vector < vector <bool>> matrix_next_gen(n, vector(n, false));

list <pair<int,int>> live_cells;


int color_muerto[] = {0,0,0};
int color_vivo[] = {255,255,255};


int index_zoom = 11;
vector <int> zoom = {7, 10, 14, 20, 25, 28, 35, 50, 70, 100, 140, 175, 350, 700};

// Definimos la fuente que vamos a ocupar dentro de la ventana
sf::Font font;

// Creamos un mutex para que al momento que ocupamos los diferentes hilos, no se pierda la informacion de las celdas que estamos dibujando
sem_t sem;
mutex bloqueo;

sf::RenderTexture inner;



pair<sf::RectangleShape, sf::Text> createButton(int szBtnX, int szBtny, int posX, int posY, string texto, int szTexto, int posTX, int posTY){
    sf::RectangleShape button(sf::Vector2f(szBtnX, szBtny));
    button.setFillColor(sf::Color(200, 200, 200));
    button.setPosition(posX, posY);

    sf::Text text(texto, font, szTexto);
    text.setFillColor(sf::Color::Black);
    text.setPosition(button.getPosition().x + posTX, button.getPosition().y + posTY);

    return std::make_pair(button, text);
}

void handleNextStep(int x1, int x2, int y1, int y2, int iteracion, bool nulo){

	if (iteracion == 0){

		for (int i = x1; i <= x2; i++){
			for (int j = y1; j <= y2; j++) {
                int poblacion = 0;
                if (nulo){
					// Lado izquierdo
					if (i - 1 >= 0){
						if (j - 1 >= 0) poblacion += (matrix[j - 1][i - 1]);
						poblacion += (matrix[j][i - 1]);
						if (j + 1 < n) poblacion += matrix[j + 1][i - 1];
					}
					// Lado central
					if (j - 1 >= 0) poblacion += matrix[j - 1][i];
					if (j + 1 < n) poblacion += matrix[j + 1][i];

					// Lado Derecho
					if (i + 1 < n){
						if (j - 1 >= 0) poblacion += matrix[j - 1][i + 1];
						poblacion += matrix[j][i + 1];
						if (j + 1 < n) poblacion += matrix[j + 1][i + 1];
					}
                }
				else{
                    int indexIzquierda = (i - 1 < 0 ? n - 1 : i - 1);
                    int indexDerecha = (i + 1 >= n ? 0 : i + 1);
                    
					int indexArriba = (j - 1 < 0 ? n - 1 : j - 1);
                    int indexAbajo = (j + 1 >= n ? 0 : j + 1);

					// Lado inzquierdo
					poblacion += matrix[indexArriba][indexIzquierda];
					poblacion += matrix[j][indexIzquierda];
					poblacion += matrix[indexAbajo][indexIzquierda];
					
					
					// Lado central
					poblacion += matrix[indexArriba][i];
					poblacion += matrix[indexAbajo][i];

					// Lado Derechp=o
					poblacion += matrix[indexArriba][indexDerecha];
					poblacion += matrix[j][indexDerecha];
					poblacion += matrix[indexAbajo][indexDerecha];

				}


				if (matrix[j][i]){
					if (poblacion >= 4 || poblacion <= 1)  matrix_next_gen[j][i] = false;
					else{
                        live_cells.PB({j,i});
                        matrix_next_gen[j][i] = true;
                    }
                }

				else {
					if (poblacion == 3){
                        live_cells.PB({j,i});
                        matrix_next_gen[j][i] = true;
                    }
                }


			}
		}

		return;
	}

    int mitad_x = x2 / 2;
	int mitad_y = y2 / 2;

	thread cuadrantes[4];
	
	cuadrantes[0] = thread (handleNextStep, x1, mitad_x, y1, mitad_y, iteracion + 1, nulo);
	cuadrantes[1] = thread (handleNextStep, mitad_x  + 1, x2, y1, mitad_y, iteracion + 1, nulo);
	cuadrantes[2] = thread (handleNextStep, x1, mitad_x, mitad_y  + 1, y2, iteracion + 1, nulo);
	cuadrantes[3] = thread (handleNextStep, mitad_x + 1, x2, mitad_y + 1, y2, iteracion + 1, nulo);

	for (int i = 0; i < 4; i++) cuadrantes[i].join();
    
    return;
}

void updateGameVisual(){

    sizeCelda_X = zoom[index_zoom];
    sizeCelda_Y = zoom[index_zoom];
    static sf :: RectangleShape celda(sf::Vector2f(sizeCelda_X, sizeCelda_Y));

    for (list<pair<int,int>>:: iterator it = live_cells.begin(); it != live_cells.end(); it++){
        int a = it -> first;
        int b = it -> second;
        celda.setPosition(a*sizeCelda_Y, b*sizeCelda_Y);
        celda.setFillColor(sf::Color(color_vivo[0], color_vivo[1], color_vivo[2]));
        inner.draw(celda);
    }
}

void actionHandler(string action){

    if (action == "Evolucion Automatica"){

    }
    if (action == "Siguiente Evolucion"){
        inner.clear(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));
        live_cells.clear();
        handleNextStep(0 , n - 1, 0, n - 1, 0, true);
        matrix = matrix_next_gen;
        matrix_next_gen = matrix_clean;
        updateGameVisual();
    }


    return;
}


int main() {

    // Glider
	matrix[1][0] = true;
	matrix[2][1] = true;
	matrix[0][2] = true;
	matrix[1][2] = true;
	matrix[2][2] = true;

    live_cells.PB({1,0});
    live_cells.PB({2,1});
    live_cells.PB({0,2});
    live_cells.PB({1,2});
    live_cells.PB({2,2});

    // Creamos la ventana principal en la cual tendra todos los botones y la ventana del juego
    sf::RenderWindow outerWindow(sf::VideoMode(1650, 880), "Conway's Game of Life");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Le asignamos las dimensiones a la pantalla del juego
    inner.create(1400, 700);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Usamos la clase Sprite para poder dibujar todas las celdas dentro de la pantalla del juego
    sf::Sprite innerSprite(inner.getTexture());

    // Calculamos la posicion para poder colocar correctamente la pantalla del juego
    int distancia_X = (outerWindow.getSize().x - inner.getSize().x)  - 20;
    int distancia_Y = ((outerWindow.getSize().y - inner.getSize().y) / 2);

    // Posicionamos la pantalla del juego
    innerSprite.setPosition(sf::Vector2f(distancia_X, distancia_Y));

    // Asignamos el color de fondo o de las celdas muertas 
    inner.clear(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  BOTONES

    // Asignamos la fuente que vamos a ocupar en el programa
    font.loadFromFile("./Fonts/arial.ttf");
    
    // Creamos todos los diferentes botones y los ponemos dentro de un vector para facilitar su dibujo en pantalla y el evento de click
    vector<std::pair<sf::RectangleShape, sf::Text>> buttons = {
        createButton(180,60, 20, 30, "Evolucion Automatica", 17, 6, 17),
        createButton(180,60, 20, 120, "Detener", 17, 55, 17),
        createButton(180,60, 20, 210, "Siguiente Evolucion", 17, 15, 17),
        createButton(60, 50, 30, 330, "+", 25, 24, 10),
        createButton(60, 50, 120, 330, "-", 25, 26, 8),
        createButton(180, 60, 20, 450, "Seleccionar Color", 17, 20, 17)
    };

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Creamos el primer estado si es que hay uno en la matriz de inicio
    updateGameVisual();
    

    // Bucle que ocupamos para la pantalla mientras ésta esté presente
    while (outerWindow.isOpen()) {
        sf::Event event;
        
        // Limpiamos la pantalla principal y le colocamos un color de fondo
        outerWindow.clear(sf::Color(92,117,140));

        // Checamos si es que en algun momento se tuvo algun evento
        while (outerWindow.pollEvent(event)) {
            // En caso de que el evento sea cerrar la ventana, cerramos completamente toda la ventana
            if (event.type == sf::Event::Closed){
                outerWindow.close();
            }

            // En caso de que el evento sea en donde se presiona el boton de el mouse, checamos que sea el izquierdo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                // Obtenemos la posicion del mouse
                sf::Vector2i mousePos = sf::Mouse::getPosition(outerWindow);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                // De todos los botones presionados checamos cual fue presionado
                for (int i = 0; i < buttons.size(); i++){
                    
                    if (buttons[i].first.getGlobalBounds().contains(mousePosF)) {
                            // Si el boton fue presionado, lo mandamos a nuestra funcion de actionHandler
                            actionHandler(buttons[i].second.getString());
                    }
                }
          }
        }

        // Primero mostramos la pantalla del juego para que no se tenga algun efecto de parpadeo
        inner.display();

        // Colocamos en la ventana principal la ventana del juego
        outerWindow.draw(innerSprite);
        
        // -------------------------------------------------------------------------------------------------------------------------
        // Colocamos los diferentes botones
        for (auto& button : buttons) {
            outerWindow.draw(button.first);
            outerWindow.draw(button.second);
        }
        // -------------------------------------------------------------------------------------------------------------------------


        // Mostramos la pantalla principal con todos los elementos que colocamos anteriormente
        outerWindow.display();
    }

    return 0;
}