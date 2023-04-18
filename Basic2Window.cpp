#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
#include <semaphore.h>
#include <chrono>
#define PB push_back
using namespace std;

int n = 700;
int sizeCelda_X, sizeCelda_Y;

// Matrices necesarias para los diferentes estados del juego
vector < vector <bool>> matrix(n, vector(n, false));
vector < vector <bool>> matrix_clean(n, vector(n, false));
vector < vector <bool>> matrix_next_gen(n, vector(n, false));

// Arreglo para las celdas que se encuentran vivas
vector <list <int>> live_cells(n);

// Conjunto para tener control de las reglas del juego
set <int> regla_nacimineto;
set <int> regla_sobrevivir;


int color_muerto[] = {0,0,0};
int color_vivo[] = {255,255,255};

int index_visual_x = 0;
int index_visual_y = 0;
int valor_scroll = 1;

int total_iteraciones = 0;
int total_celdas_vivas = 0;


int index_zoom = 13;
vector <int> zoom = {/*1, 2,*/ 4, 5, 7, 10, 14, 20, 25, 28, 35, 50, 70, 100, 140, 175, 350, 700};

bool bandera_automatico = false;
bool bandera_nulo = true;

// Definimos la fuente que vamos a ocupar dentro de la ventana
sf::Font font;
sf::RenderTexture inner;

const float margin = 10.f; // Margin size in pixels


pair<sf::RectangleShape, sf::Text> createButton(int szBtnX, int szBtny, int posX, int posY, string texto, int szTexto, int posTX, int posTY){
    sf::RectangleShape button(sf::Vector2f(szBtnX, szBtny));
    button.setFillColor(sf::Color(200, 200, 200));
    button.setPosition(posX, posY);

    sf::Text text(texto, font, szTexto);
    text.setFillColor(sf::Color::Black);
    text.setPosition(button.getPosition().x + posTX, button.getPosition().y + posTY);

    return std::make_pair(button, text);
}

void handleNextStep(int x1, int x2, int y1, int y2, int iteracion){
    total_iteraciones++;
    total_celdas_vivas = 0;

	if (iteracion == 0){

		for (int i = x1; i <= x2; i++){
			for (int j = y1; j <= y2; j++) {
                int poblacion = 0;
                if (bandera_nulo){
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


                // Sobrevivir
				if (matrix[j][i]){
                    // Hacemos un simple chequeo de que la cantidad de celdas de esa region sea parte de 
                    // la regla para sobrevivir
                    if (regla_sobrevivir.count(poblacion)){
                        live_cells[j].PB(i);
                        matrix_next_gen[j][i] = true;
                        total_celdas_vivas++;
                    }
                    else matrix_next_gen[j][i] = false;
                }

				else { // Nacimineto
                    if (regla_nacimineto.count(poblacion)){
                        live_cells[j].PB(i);
                        matrix_next_gen[j][i] = true;
                        total_celdas_vivas++;
                    }
                }
			}
		}

		return;
	}

    int mitad_x = x2 / 2;
	int mitad_y = y2 / 2;

	thread cuadrantes[4];
	
	cuadrantes[0] = thread (handleNextStep, x1, mitad_x, y1, mitad_y, iteracion + 1);
	cuadrantes[1] = thread (handleNextStep, mitad_x  + 1, x2, y1, mitad_y, iteracion + 1);
	cuadrantes[2] = thread (handleNextStep, x1, mitad_x, mitad_y  + 1, y2, iteracion + 1);
	cuadrantes[3] = thread (handleNextStep, mitad_x + 1, x2, mitad_y + 1, y2, iteracion + 1);

	for (int i = 0; i < 4; i++) cuadrantes[i].join();
    
    return;
}

void updateGameVisual(){
    inner.clear(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));

    total_celdas_vivas = 0;

    sizeCelda_X = zoom[index_zoom];
    sizeCelda_Y = zoom[index_zoom];

    int cantidad_x = 1400/sizeCelda_X;
    int cantidad_y = 700/sizeCelda_Y;

    sf :: RectangleShape celda(sf::Vector2f(sizeCelda_X, sizeCelda_Y));
    int index = index_visual_x;

    while(cantidad_x--){
        if (live_cells[index].size()) {
            for (list<int>:: iterator it = live_cells[index].begin(); it != live_cells[index].end(); it++){
                if (*it > index_visual_y + cantidad_y) break;

                celda.setPosition((index-index_visual_x)*sizeCelda_X, (*(it) - index_visual_y)*sizeCelda_Y);
                celda.setFillColor(sf::Color(color_vivo[0], color_vivo[1], color_vivo[2]));
                inner.draw(celda);
                total_celdas_vivas++;
            }
        } 

        index++;
    }

    return;
}

void selectColor(int opcion){

    // Creamos la venta

    string titutlo = "";

    if (opcion == 1) titutlo = "vivas ";
    else titutlo = "muertas ";

    sf::RenderWindow windowSelectColor(sf::VideoMode(755, 400), "Selecciona un color para las celdas " + titutlo);
    sf::Color selectedColor = sf::Color::White;
    
    // Creamos la dimension que va a tener cada color muestra
    const int tileSize = 40;

    vector<sf::RectangleShape> palette;
    vector<sf::RectangleShape> palette_clean;
    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));

    int r, g, b, y = 20;

    for(int i = 0; i < 4; i++) {
        int x = 20;
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                tile.setPosition(x, y);

                bitset<8> binary_num;
                
                binary_num = (i << 6 | j << 4 | k << 2);
                r = binary_num.to_ulong();

                binary_num = (i << 4 | j << 2 | k << 6);
                g = binary_num.to_ulong();


                binary_num = (i << 2 | j << 6 | k << 4);
                b = binary_num.to_ulong();

                x += 45;

                sf::Color color(r, g, b);

                tile.setFillColor(color);
                palette.push_back(tile);

            }

        }
        y += 45;
    } 

    palette_clean = palette;

    auto buttonOk = createButton(100, 50, 327, 300, "OK", 24, 32, 10);

    int pressed = -1;

    while (windowSelectColor.isOpen()) {
        sf::Event event;
        while (windowSelectColor.pollEvent(event)) {
            if (event.type == sf::Event::Closed) windowSelectColor.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                
                // Get the mouse position relative to the window
                sf::Vector2i mousePosition = sf::Mouse::getPosition(windowSelectColor);

                // Hacemos un recorrido por todos los colores disponibles y checamos si alguno de ellos fue presionado
                for (int i = 0; i < palette.size(); ++i){
                    if (palette[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y)){
                        // En caso de que algun color sea presioado, ocupamos la pelata de colores sin ningun cambio y la asignamos a la que
                        // vamos a mostrar
                        palette = palette_clean;

                        // Asignamos el color que se selecciono en una variable
                        selectedColor = palette[i].getFillColor();

                        // Del color seleccionado le agregamos una ayuda visual para mostrar en que posicion se encuentra
                        palette[i].setSize(palette[i].getSize() - sf::Vector2f(margin * 2.f, margin * 2.f));
                        palette[i].setPosition(palette[i].getPosition() + sf::Vector2f(margin, margin));
                    }
                }


                // Una vez presionado el boton de OK, asignamos el color a la celda
                if (buttonOk.first.getGlobalBounds().contains(mousePosition.x, mousePosition.y)){
                    r = (int)selectedColor.r;
                    g = (int)selectedColor.g;
                    b = (int)selectedColor.b;

                    // Unicamente checamos si tenemos que asignar el color a la celda viva o muerta
                    if (opcion == 1){ 
                        color_vivo[0] = r;
                        color_vivo[1] = g;
                        color_vivo[2] = b;
                    }
                    else {
                        color_muerto[0] = r;
                        color_muerto[1] = g;
                        color_muerto[2] = b;
                    }
                    windowSelectColor.close();
                }
            }
        }

        // Colocamos todos los elementos de la pantalla y los mostramos

        windowSelectColor.clear(sf::Color::White);
        windowSelectColor.draw(buttonOk.first);
        windowSelectColor.draw(buttonOk.second);


        for (int i = 0; i < palette.size(); ++i) windowSelectColor.draw(palette[i]);

        windowSelectColor.display();
    }
}

void updateColors(){
    // Creamos la ventana donde manejaremos el cambio de color de las celdas
    sf::RenderWindow windowColor(sf::VideoMode(660, 250), "Cambio de color");

    auto buttonOk = createButton(100, 50, 280, 170, "OK", 24, 32, 10);

    sf::RectangleShape alive(sf::Vector2f(200, 80));
    sf::RectangleShape death(sf::Vector2f(200, 80));

    alive.setFillColor(sf::Color(color_vivo[0], color_vivo[1], color_vivo[2]));
    death.setFillColor(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));

    alive.setPosition(65, 50);
    death.setPosition(395, 50);

    sf::Text text1("Celdas Vivas", font, 24);
    text1.setFillColor(sf::Color::Black);
    text1.setPosition(90, 15);

    sf::Text text2("Celdas Muertas", font, 24);
    text2.setFillColor(sf::Color::Black);
    text2.setPosition(405, 15);



    while (windowColor.isOpen()){
        sf::Event event;
        while (windowColor.pollEvent(event)){
            if (event.type == sf::Event::Closed) windowColor.close();

            // En caso de que el evento sea en donde se presiona el boton de el mouse, checamos que sea el izquierdo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                // Obtenemos la posicion del mouse
                sf::Vector2i mousePos = sf::Mouse::getPosition(windowColor);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (buttonOk.first.getGlobalBounds().contains(mousePosF)) windowColor.close();
                if (alive.getGlobalBounds().contains(mousePosF)){
                    // Cambiamos el color vivo
                    windowColor.setActive(false);
                    selectColor(1);
                    alive.setFillColor(sf::Color(color_vivo[0], color_vivo[1], color_vivo[2]));
                    windowColor.setActive(true);

                }

                if (death.getGlobalBounds().contains(mousePosF)){
                    // Cambiamos el color vivo
                    windowColor.setActive(false);
                    selectColor(2);
                    death.setFillColor(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));
                    windowColor.setActive(true);
                }

            }
        }

        windowColor.clear(sf::Color(247,155,131));

        // Draw the preview rectangle and the palette to the window
        windowColor.draw(alive);
        windowColor.draw(death);

        windowColor.draw(text1);
        windowColor.draw(text2);

        windowColor.draw(buttonOk.first);
        windowColor.draw(buttonOk.second);

        windowColor.display();
    }

   return;
}

void nueva_regla(){
    sf::RenderWindow windowRegla(sf::VideoMode(500, 300), "Nueva regla");
    // Limpiamos la pantalla principal y le colocamos un color de fondo
    windowRegla.clear(sf::Color(92,117,140));

    auto botonOK = createButton(100, 60, 120, 200, "OK", 20, 30, 16);
    auto botonCancelar = createButton(100, 60, 280, 200, "Cancelar", 20, 8, 16);


    sf::RectangleShape inputBox(sf::Vector2f(300, 50));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setPosition(100, 100);

    sf::Text text("|", font, 24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(104, 108);

    string input;
    string mostrar;
    int index = 0;
    bool banderaOk = false;

    while (windowRegla.isOpen()){
        sf::Event event;
        windowRegla.clear(sf::Color(92,117,140));

        while (windowRegla.pollEvent(event)){
            if (event.type == sf::Event::Closed) windowRegla.close();

            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b'){
                    // Hacemos el manejo de borrar el ultimo caracter
                    if (!input.empty()){
                        input.pop_back();
                        index = max(0, index - 1);;
                    }
                }
                // Si alguna tecla del alfabeto fue presionada
                else if (event.text.unicode < 128) {
                    // La agregamos a la cadena que vamos a mostrar
                    input += static_cast<char>(event.text.unicode);
                }

                if (input.size() > 21){
                    int temp = input.size();
                    mostrar = input.substr(index, min(21, temp));
                }
                else mostrar = input;

                text.setString(mostrar);
            }

            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Get the mouse position relative to the window
                sf::Vector2i mousePosition = sf::Mouse::getPosition(windowRegla);

                // Una vez presionado el boton de OK, asignamos el color a la celda
                if (botonOK.first.getGlobalBounds().contains(mousePosition.x, mousePosition.y)){
                    banderaOk = true;
                    windowRegla.close();
                }
                if (botonCancelar.first.getGlobalBounds().contains(mousePosition.x, mousePosition.y)){
                    windowRegla.close();
                }
            }
        }

            windowRegla.draw(inputBox);
            windowRegla.draw(text);
            windowRegla.draw(botonOK.first);
            windowRegla.draw(botonOK.second);

            windowRegla.draw(botonCancelar.first);
            windowRegla.draw(botonCancelar.second);

            windowRegla.display();
    }


    if (input.size() > 3 && banderaOk){
        // Limpiamos los conjuntos para poder agregar los valores de la nueva regla
        regla_nacimineto.clear();
        regla_sobrevivir.clear();
        
        string survivial = "";
        string birth = "";
        bool bandera = false;

        for (int i = 0; i < input.size(); i++){
            if (input[i] == '/') bandera = true;

            if (bandera) survivial += input[i];
            else birth += input[i];
        }

        for (int i = 0; i < birth.size(); i++)
            if (birth[i] >= '0' && birth[i] <= '9') regla_nacimineto.insert(birth[i]-'0');
        
        for (int i = 0; i < survivial.size(); i++)
            if (survivial[i] >= '0' && survivial[i] <= '9') regla_sobrevivir.insert(survivial[i]-'0');
    }

    return;
}

void FileHandler(){

	freopen("alchemy_input.txt", "r", stdin);
	freopen("alchemy_output.txt", "w", stdout);
 
    return;
}

void actionHandler(string action){

    if (action == "Evolucion Automatica" || action == "Siguiente Evolucion") {
        if (action == "Evolucion Automatica") {
            bandera_automatico = true;
            this_thread::sleep_for(chrono::milliseconds(10));
        } 
        else bandera_automatico = false;

        live_cells.clear();
        live_cells.resize(n);

        handleNextStep(0 , n - 1, 0, n - 1, 0);
        matrix = matrix_next_gen;
        matrix_next_gen = matrix_clean;
        updateGameVisual();
    }

    if (action == "Detener") bandera_automatico = false;

    if (action == "+" || action == "-") {
        int delta = (action == "+" ? 1 : -1);
        index_zoom += (index_zoom + delta < zoom.size() && index_zoom + delta >= 0 ? delta : 0);
        updateGameVisual();
    }

    if (action == "Toro" || action == "Nulo") bandera_nulo = (action == "Nulo") ? true : false;

    if (action == ">"){
        index_visual_x += (index_visual_x + valor_scroll < n) ? valor_scroll : 0;
        updateGameVisual();
    }

    if (action == "<"){
        index_visual_x -= (index_visual_x - valor_scroll >= 0 ) ? valor_scroll : 0;
        updateGameVisual();
    }

    if (action == "v"){
        index_visual_y += (index_visual_y + valor_scroll < n) ? valor_scroll : 0;
        updateGameVisual();
    }

    if (action == "^"){
        index_visual_y -= (index_visual_y - valor_scroll >= 0) ? valor_scroll : 0;
        updateGameVisual();
    }

    if (action == "Limpiar Juego"){
        matrix = matrix_clean;
        live_cells.clear();
        live_cells.resize(n);
        updateGameVisual();
    }

    if (action == "Inicializar Juego"){  
        matrix = matrix_clean;
        live_cells.clear();
        live_cells.resize(n);

         // seed the random number generator with the current time
        srand(time(NULL));
    
        // fill the grid with random values
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if ((rand() % 2) & 1){
                     matrix[j][i] = true;
                     live_cells[j].PB(i);
                }
                else matrix[j][i] = false;

            }
        }

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

    live_cells[1].PB(0);
    live_cells[2].PB(1);
    live_cells[0].PB(2);
    live_cells[1].PB(2);
    live_cells[2].PB(2);

    // Creamos la ventana principal en la cual tendra todos los botones y la ventana del juego
    sf::RenderWindow outerWindow(sf::VideoMode(1650, 880), "Conway's Game of Life");

    // Le asignamos las dimensiones a la pantalla del juego
    inner.create(1400, 700);

    // Usamos la clase Sprite para poder dibujar todas las celdas dentro de la pantalla del juego
    sf::Sprite innerSprite(inner.getTexture());

    // Calculamos la posicion para poder colocar correctamente la pantalla del juego
    int distancia_X = (outerWindow.getSize().x - inner.getSize().x)  - 20;
    int distancia_Y = ((outerWindow.getSize().y - inner.getSize().y) / 2);

    // Posicionamos la pantalla del juego
    innerSprite.setPosition(sf::Vector2f(distancia_X, distancia_Y));

    // Asignamos el color de fondo o de las celdas muertas 
    inner.clear(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));

// BOTONES  ------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Asignamos la fuente que vamos a ocupar en el programa
    font.loadFromFile("./Fonts/arial.ttf");
    
    // Creamos todos los diferentes botones y los ponemos dentro de un vector para facilitar su dibujo en pantalla y el evento de click
    vector<std::pair<sf::RectangleShape, sf::Text>> buttons = {
        createButton(180,60, 20, 30, "Evolucion Automatica", 17, 6, 17),
        createButton(180,60, 20, 120, "Detener", 17, 55, 17),
        createButton(180,60, 20, 210, "Siguiente Evolucion", 17, 15, 17),
        createButton(60, 50, 30, 300, "-", 25, 26, 8),
        createButton(60, 50, 120, 300, "+", 25, 24, 10),

        createButton(180, 50, 1200, 20, "Inicializar Juego", 17, 20, 14),
        createButton(180, 50, 1450, 20, "Limpiar Juego", 17, 36, 14),

        createButton(60, 30, 300, 30, "Toro", 20, 6, 2),
        createButton(60, 30, 370, 30, "Nulo", 20, 6, 2),

        createButton(50, 45, 80, 400, "^", 32, 16, 5),
        createButton(50, 45, 80, 450, "v", 24, 18, 5),
        createButton(50, 40, 20, 425, "<", 24, 17, 5),
        createButton(50, 40, 140, 425, ">", 24, 17, 5),

        createButton(180, 60, 20, 620, "Seleccionar Color", 17, 20, 17),
        createButton(180, 60, 20, 710, "Definir regla B/S", 17, 22, 18),

        createButton(80, 50, 20, 800, "Guardar", 14, 15, 17),
        createButton(80, 50, 110, 800, "Abrir", 14, 25  , 17)
    };
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Definimos las reglas iniciales del juego
    regla_nacimineto.insert(3);
    regla_sobrevivir.insert(2);
    regla_sobrevivir.insert(3);

    // Agregamos los recuadros para mostrar la cantidad de celdas vivas y la iteracion actual

    sf::RectangleShape label_iteraciones(sf::Vector2f(200, 50));
    label_iteraciones.setFillColor(sf::Color(194, 196, 208));
    label_iteraciones.setPosition(400, 810);

    sf::Text text_iteraciones("Iteracion : " + to_string(total_iteraciones), font, 20);
    text_iteraciones.setFillColor(sf::Color::Black);
    text_iteraciones.setPosition(label_iteraciones.getPosition().x + 5, label_iteraciones.getPosition().y + 13);


    sf::RectangleShape label_celdas_vivas(sf::Vector2f(200, 50));
    label_celdas_vivas.setFillColor(sf::Color(194, 196, 208));
    label_celdas_vivas.setPosition(650, 810);

    sf::Text text_celdas_vivas("Celdas vivas: " + to_string(total_celdas_vivas), font, 18);
    text_celdas_vivas.setFillColor(sf::Color::Black);
    text_celdas_vivas.setPosition(label_celdas_vivas.getPosition().x + 5, label_celdas_vivas.getPosition().y + 13);


    // Bucle que ocupamos para la pantalla mientras ésta esté presente
    while (outerWindow.isOpen()) {
       //  updateGameVisual();
        sf::Event event;
        
        // Limpiamos la pantalla principal y le colocamos un color de fondo
        outerWindow.clear(sf::Color(92,117,140));

        // Checamos si es que en algun momento se tuvo algun evento
        while (outerWindow.pollEvent(event)) {
            // En caso de que el evento sea cerrar la ventana, cerramos completamente toda la ventana
            if (event.type == sf::Event::Closed)  outerWindow.close();

            // En caso de que el evento sea en donde se presiona el boton de el mouse, checamos que sea el izquierdo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                // Obtenemos la posicion del mouse
                sf::Vector2i mousePos = sf::Mouse::getPosition(outerWindow);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                // De todos los botones presionados checamos cual fue presionado
                for (int i = 0; i < buttons.size(); i++){
                    
                    if (buttons[i].first.getGlobalBounds().contains(mousePosF)) {
                            // Si el boton fue presionado, lo mandamos a nuestra funcion de actionHandler
                            string action = buttons[i].second.getString();

                            if (action == "Seleccionar Color"){ 
                                outerWindow.setVisible(false);
                                updateColors();
                                outerWindow.setVisible(true);
                            }
                            else if (action == "Definir regla B/S"){
                                outerWindow.setVisible(false);
                                nueva_regla();
                                outerWindow.setVisible(true);
                            }

                            else actionHandler(action);
                    }
                }


                // Dentro de la sigueinte condicional, checamos si es que se presiono alguna celda dentro del tablero
                // de esa manera saber que celda debemos cambiar su estado
                if (innerSprite.getGlobalBounds().contains(mousePosF) && !bandera_automatico) {
                    // Primero debemos encotnrar las coordenadas de la celda presionada
                    int x = (int)mousePosF.x - 230;
                    int y = (int)mousePosF.y - 90;

                    // Asignamos el tama;o de la celda y para poder crearla 
                    sizeCelda_X = zoom[index_zoom];
                    sizeCelda_Y = zoom[index_zoom];

                    // Como estamos ocupando una matrix para el estado de las celdas, debemos saber los indices de esas celdas
                    int index_x = x / sizeCelda_X;
                    int index_y = y / sizeCelda_Y;

                    // Por como se manejan las celdas, es necesario hacer el siguiente paso
                    swap(index_x, index_y);


                    // Cambiamos la celda a su estado contrario
                    if (matrix[index_y][index_x] ) matrix[index_y][index_x] = false;
                    else  matrix[index_y][index_x] = true;

                    // En caso de que la celda este viva, hay que agregarla al arreglo que ocupamos para dibujar todo el juego
                    // Pero en ambos casos dibujamos la celda en el tablero

                    if (matrix[index_y][index_x]) {
                        live_cells[index_y].PB(index_x); 
                        total_celdas_vivas++;
                    }
                    else{ // En caso contrario, tenemos que quitar esa misma celda
                        for (list<int>:: iterator it = live_cells[index_y].begin(); it != live_cells[index_y].end(); it++){
                            if (*it == index_x){
                                live_cells[index_y].erase(it);
                                // Creamos la celda, la posicionamos, ponemos color y la ponemos en el tablero 
                                sf :: RectangleShape celda(sf::Vector2f(sizeCelda_X, sizeCelda_Y));
                                celda.setPosition(index_y*sizeCelda_X, index_x*sizeCelda_Y);
                                celda.setFillColor(sf::Color(color_muerto[0], color_muerto[1], color_muerto[2]));
                                inner.draw(celda);
                                total_celdas_vivas--;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (bandera_automatico) actionHandler("Evolucion Automatica");

        updateGameVisual();

        // Primero mostramos la pantalla del juego para que no se tenga algun efecto de parpadeo
        inner.display();

        // Colocamos en la ventana principal la ventana del juego
        outerWindow.draw(innerSprite);
        
        // -------------------------------------------------------------------------------------------------------------------------
        // Colocamos los diferentes botones
        for (auto& button : buttons) {
            outerWindow.draw(button.first);

            if (button.second.getString() == "Nulo"){
                if (bandera_nulo) button.first.setFillColor(sf::Color(96, 96, 96));
                else button.first.setFillColor(sf::Color(200, 200, 200));
            }

            if (button.second.getString() == "Toro"){
                if (!bandera_nulo) button.first.setFillColor(sf::Color(96, 96, 96));
                else button.first.setFillColor(sf::Color(200, 200, 200));
            }



            outerWindow.draw(button.second);
        }
        // -------------------------------------------------------------------------------------------------------------------------


        text_iteraciones.setString("Iteracion : " + to_string(total_iteraciones));
        text_celdas_vivas.setString("Celdas vivas : " + to_string(total_celdas_vivas));


        outerWindow.draw(label_iteraciones);
        outerWindow.draw(text_iteraciones);

        outerWindow.draw(label_celdas_vivas);
        outerWindow.draw(text_celdas_vivas);


        // Mostramos la pantalla principal con todos los elementos que colocamos anteriormente
        outerWindow.display();
    }

    return 0;
}