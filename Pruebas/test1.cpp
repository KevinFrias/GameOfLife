#include <gtk/gtk.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

// sudo apt-get install libgtk-3-dev
// g++ test1.cpp `pkg-config --cflags --libs gtk+-3.0` -lsfml-graphics -lsfml-window -lsfml-system -o program

void open_file() {
    cout << "Abrimos archivo" << endl;

    while(true){
        /*
        GtkWidget *dialog;
        GtkFileFilter *filter;
        GtkFileChooser *chooser;
        GtkResponseType response;


        gtk_init(NULL, NULL);
        dialog = gtk_file_chooser_dialog_new("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

        // Add a filter for text files
        filter = gtk_file_filter_new();
        gtk_file_filter_add_pattern(filter, "*.txt");
        gtk_file_filter_set_name(filter, "Text files");
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

        response = gtk_dialog_run(GTK_DIALOG(dialog));

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            string filename;
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            cout << filename << endl;
        } 
        
        gtk_widget_destroy(dialog);
        */

        break;
    }

    return;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Buttons");

    // Create the buttons
    sf::RectangleShape openButton(sf::Vector2f(100, 50));
    openButton.setFillColor(sf::Color::Green);
    openButton.setPosition(25, 50);

    sf::RectangleShape saveButton(sf::Vector2f(100, 50));
    saveButton.setFillColor(sf::Color::Blue);
    saveButton.setPosition(25, 125);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check if the mouse button was pressed
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if the open button was pressed
                if (openButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cout << "Open the file" << endl;
                    open_file();
                }

                // Check if the save button was pressed
                if (saveButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cout << "Save the file" << endl;
                }
            }
        }

        window.clear();
        window.draw(openButton);
        window.draw(saveButton);
        window.display();

        // Sleep for a short time to avoid using too much CPU
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return 0;
}