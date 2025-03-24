#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <SFML/Window.hpp>
#include <Windows.h> // Für SetWindowPos()
#include <random>
#include <ctime>
#include <cmath>
#include "Button.hpp"
#include "cwindow.hpp"
#include "windows.hpp"
#include "globals.hpp"

using namespace sf;

void gameOver();
void gameLoop();
void newWindow();
void openMenu();
void initRandom();
void clicker();
void boxCounter();
void painter();
void fillUp();
int getRandInt(int min, int max);

const int maxWindows = 7;
static Font font;
std::vector<std::thread> threads;



#pragma region Manager
int main()
{
    initRandom();

    openMenu();

    return 0;
}

void openMenu()
{
    //Fenster erstllen
    auto menuWindow = RenderWindow(sf::VideoMode({500, 500}), "C-Error", sf::Style::Titlebar | sf::Style::Close);
    menuWindow.setFramerateLimit(90);

    //Variablen & font setzen
    active = true;
    float winMid = menuWindow.getSize().x / 2;
    if (!font.openFromFile("./assets/RobotoMono-Regular.ttf")) std::cerr << "Cannot load font" << std::endl;  

    //Start Button
    Button startButton(winMid, 250, 200, 60, "start Game");

    //Titel text
    Text title(font, "C-Error", 24);
    title.setFillColor(Color::White);
    FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({titleBounds.size.x / 2, titleBounds.size.y / 2});
    float y = 100;
    title.setPosition({winMid, y});

    //Score Text
    Text textScore(font, "Your Score: " + std::to_string(finishedWindows), 16);
    textScore.setFillColor(Color::White);
    FloatRect textScoreBounds = textScore.getLocalBounds();
    textScore.setOrigin({textScoreBounds.size.x / 2, textScoreBounds.size.y / 2});
    y = 300;
    textScore.setPosition({winMid, y});

    //Escape text
    Text text(font, "You can alwayse escape the game, using ESCAPE.", 16);
    text.setFillColor(Color::White);
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2, textBounds.size.y / 2});
    y = 350;
    text.setPosition({winMid, y});
    

    menuWindow.clear();
    menuWindow.draw(title);
    menuWindow.draw(textScore);
    menuWindow.draw(text);
    startButton.draw(menuWindow);
    menuWindow.display();

    while (menuWindow.isOpen())
    {
        while (const std::optional event = menuWindow.pollEvent())
        {
            //Fenster schließen
            if (event->is<sf::Event::Closed>() || Keyboard::isKeyPressed(Keyboard::Key::Escape))
            {
                active = false;
                menuWindow.close();
            }
        }

        //Spielstart durch Button Click
        if (startButton.clickDown(menuWindow))
        {
            menuWindow.close();
            gameLoop();
        }
    }
}

void gameOver()
{
    std::cout << "Game Over!" << std::endl;

    //Es wird gewartet bis alle threads beendet sind
    for (auto& th : threads) {
        if (th.joinable())
        {
            th.join();
        }
    }

    openMenu();
}

void gameLoop()
{
    //Variablen setzen
    active = true;
    float difTime = 5;
    finishedWindows = 0;

    //Uhr initialisieren und starten
    Clock clock;
    clock.start();

    //Erstes Window erstellen
    newWindow();

    //gameloop
    while (active)
    {
        //Spiel abbrechen bei ESCAPE
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            active = false;
        }

        //Neues Window erstellen
        if ((clock.getElapsedTime().asSeconds() > difTime && openWindows <= maxWindows) || openWindows == 0)
        {
            //Game Over bedingung
            if (openWindows >= maxWindows)
            {
                active = false;
            }

            clock.restart();
            newWindow();

            //Neue Zeit zum nächsten Fenster berechnen
            difTime = static_cast<float>(pow(0.945, finishedWindows- 20)) + 1;
        }
    }

    gameOver();
}

void newWindow()
{
    openWindows += 1;

    //Zufälliges Fenster öffnen
    int window = getRandInt(0,3);
    if (window == 0) threads.push_back(std::thread(clicker));
    if (window == 1) threads.push_back(std::thread(boxCounter));
    if (window == 2) threads.push_back(std::thread(painter));
    if (window == 3) threads.push_back(std::thread(fillUp));
    
    return;
}

void clicker() 
{
    Clicker clicker;
}

void boxCounter()
{
    BoxCounter boxCounter;
}

void painter()
{
    Painter painter;
}

void fillUp()
{
    FillUp fillUp;
}


#pragma endregion


/*
int windowPrefab() 
{
    RenderWindow window = createDefaultWindow("WindowName");
    RectangleShape headerRect = createDefaultHeaderRect(window.getSize());
    Text headerText = createDefaultHeaderText(window.getSize(), "WindowDescription");
    

    bool firstFrame = true; //Damit das Fenster einmal am Anfang lädt auch wenn es nicht im Fokus ist
    while (window.isOpen())
    {
        //Event loop, damit es nicht zu Fehlern kommt
        while (const std::optional event = window.pollEvent())
        {        }
        
        if (active == false) 
        {
            window.close();
        }

        if (window.hasFocus() || firstFrame)
        {
            firstFrame = false;

            //darstellen
            window.clear();

            window.draw(headerRect);
            window.draw(headerText);
            window.display();
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
    return 0;
}
    */