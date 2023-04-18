#include <SFML/Graphics.hpp>
#include <gtk/gtk.h>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// sudo apt-get install libgtk-3-dev
// g++ test1.cpp `pkg-config --cflags --libs gtk+-3.0` -lsfml-graphics -lsfml-window -lsfml-system -o program

void on_file_chooser_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        cout << "We read the entire file" << endl;
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));

    cout << "end of function" << endl;
    gtk_main_quit(); // End the main loop and exit the program
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
        window.clear();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Check if the mouse button was pressed
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if the open button was pressed
                if (openButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cout << "Before function" << endl;
                    window.setVisible(false);

                    gtk_init(NULL, NULL);

                    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
                    GtkFileFilter *filter;

                    // Add a filter for text files
                    filter = gtk_file_filter_new();
                    gtk_file_filter_add_pattern(filter, "*.txt");
                    gtk_file_filter_set_name(filter, "Text files");
                    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
                    
                    gtk_widget_show_all(dialog);

                    // Connect the response signal to the on_file_chooser_response function
                    g_signal_connect(dialog, "response", G_CALLBACK(on_file_chooser_response), NULL);

                    gtk_main();

                    cout << "After function" << endl;

                    window.setVisible(true);
                }

                // Check if the save button was pressed
                if (saveButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cout << "Save the file" << endl;
                }
            }
        }

       


        window.draw(openButton);
        window.draw(saveButton);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
