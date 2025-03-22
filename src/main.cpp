#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <SFML/Window.hpp>
#include <Windows.h> // Für SetWindowPos()
#include <random>
#include <ctime>
#include "Button.hpp"
#include "cwindow.hpp"
#include "windows.hpp"
#include "globals.hpp"

using namespace sf;

int gameOver();
int gameLoop();
int newWindow();

void initRandom();
int clicker();
int boxCounter();
int painter();
int getRandInt(int min, int max);

const int maxWindows = 7;
//std::mt19937 rng; //random Funktion
static Font font;
std::vector<std::thread> threads;



#pragma region Manager
int main()
{
    initRandom();

    auto menuWindow = sf::RenderWindow(sf::VideoMode({500, 500}), "C-Error");
    menuWindow.setFramerateLimit(144);

    if (!font.openFromFile("./assets/RobotoMono-Regular.ttf")) std::cerr << "Cannot load font" << std::endl;

    Button startButton(300, 250, 200, 60, "Spiel Starten");
    
    while (menuWindow.isOpen())
    {
        while (const std::optional event = menuWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || Keyboard::isKeyPressed(Keyboard::Key::Escape))
            {
                active = false;
                menuWindow.close();
            }
        }

        if (startButton.clickDown(menuWindow))
        {
            menuWindow.close();
            gameLoop();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Space))
        {
            std::cout << getRandInt(0, 10) << getRandInt(0, 10) << getRandInt(0, 1) << std::endl;
        }

        menuWindow.clear();
        startButton.draw(menuWindow);
        menuWindow.display();
    }

    //Es wird gewartet bis alle threads beendet sind
    for (auto& th : threads) {
        if (th.joinable())
        {
            th.join();
        }
    }

    gameOver();
}
/*
void initRandom() {
    std::random_device rd;
    rng.seed(rd()); 
}

int getRandInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max); // Gleichverteilung
    return dist(rng);
}
*/
int gameOver()
{
    std::cout << "Game Over!" << std::endl;
    return 1;
}

int gameLoop()
{
    active = true;
    float difTime = 4;

    Clock clock;
    clock.start();

    newWindow();

    while (active)
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            active = false;
        }

        if (clock.getElapsedTime().asSeconds() > difTime && openWindows < maxWindows)
        {
            clock.restart();
            newWindow();
        }
    }

    return 0;
}

int newWindow()
{
    openWindows += 1;

    //Zufälliges Fenster öffnen
    int window = getRandInt(1,1);
    if (window == 0) threads.push_back(std::thread(clicker));
    if (window == 1) threads.push_back(std::thread(boxCounter));
    if (window == 2) threads.push_back(std::thread(painter));
    
    return 0;
}

int clicker() 
{
    Clicker clicker;
    return 0;
}

int boxCounter()
{
    BoxCounter boxCounter;
    return 0;
}

int painter()
{
    Painter painter;
    return 0;
}


#pragma endregion

#pragma region windows

/*/
int clicker() 
{
    RenderWindow window = createDefaultWindow("Clicker");
    RectangleShape headerRect = createDefaultHeaderRect(window.getSize());
    Text headerText = createDefaultHeaderText(window.getSize(), "Click on the blue rect!");
    
    int rectAmount = getRandInt(10, 30);
    std::vector<Button> wrongButtons;

    //Rechtecke erstellen und random positionieren
    int rectHeight = 40;
    int rectWidth = 40;
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        //falsche Button setzen
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight));
        Button button(x,y, rectWidth, rectHeight, "", colors.at(i%colorSize));
        wrongButtons.push_back(button);
    }

    //richtigen Button setzen
    float x = getRandInt(0, (windowWidth - rectWidth));
    float y = getRandInt(90, (windowHeight - rectHeight));
    Button rightButton(x,y, rectWidth, rectHeight, "", Color::Blue);

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
            //Window schließen wenn der richtige Button gedrückt wurde
            if (rightButton.clickDown(window))
            {
                window.close();
            }

            //geklickte Butons ausschalten
            for (Button& b : wrongButtons) {
                if (b.clickDown(window))
                {
                    b.setVisibility(false); 
                    b.setActive(false);
                }
            }

            //darstellen
            window.clear();
            for (Button& b : wrongButtons) {
                b.draw(window);
            }
            rightButton.draw(window);
            window.draw(headerRect);
            window.draw(headerText);
            window.display();
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
    return 0;
}

int boxCouter() 
{
    RenderWindow window = createDefaultWindow("box count");
    RectangleShape headerRect = createDefaultHeaderRect(window.getSize(), 300);
    Text headerText = createDefaultHeaderText(window.getSize(), "count all boxes");

    //Zähler-Text erstellen
    const int textSize = 24;
    Text numberText(font, "0", textSize);
    numberText.setFillColor(Color::White);
    FloatRect textBounds = numberText.getLocalBounds();
    numberText.setOrigin({textBounds.size.x / 2,textBounds.size.y / 2});
    float x = (window.getSize().x / 2);
    float y = window.getSize().y - 50;
    numberText.setPosition({x, y});

    int rectAmount = getRandInt(5, 15);
    std::vector<RectangleShape> rects;

    //Buttons 
    Button moreButton(x + 50, y - 10, 30, 30, ">", Color::Blue);
    Button lessButton(x - 50, y - 10, 30, 30, "<", Color::Blue);
    Button confirmButton(x, y + 20, 80, 30, "confirm", Color::Blue);

    //Rechtecke erstellen und random positionieren
    float rectHeight = 40;
    float rectWidth = 40;
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight - textSize));
        RectangleShape rect(Vector2f(rectWidth, rectHeight));
        rect.setOrigin({rectWidth / 2, rectHeight / 2});
        rect.setPosition({x,y});
        rect.setFillColor(colors.at(i%colorSize));
        rects.push_back(rect);
    }

    int counter = 0;

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

            if (counter == rectAmount && Keyboard::isKeyPressed(Keyboard::Key::Enter))

            if (moreButton.isTouched(window))// && counter < 99)
            {
                counter += 1;
                std::cout << "MORE_Button";
            }
            if (lessButton.clickDown(window) && counter > 0)
            {
                counter -= 1;
            }
            if (confirmButton.clickDown(window))
            {
                if (counter == rectAmount)
                {
                    window.close();
                }
            }

            numberText.setString(std::to_string(counter));

            //darstellen
            window.clear();
            for (RectangleShape& r : rects) {
                window.draw(r);
            }
            
            lessButton.draw(window);
            moreButton.draw(window);
            confirmButton.draw(window);
            window.draw(numberText);
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