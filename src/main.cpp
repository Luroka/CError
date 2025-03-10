#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <ctime>   // Für time()
#include "Button.hpp"

using namespace sf;


int gameOver();
int gameLoop();
int clicker();
int newWindow();
void initRandom();
int getRandInt(int min, int max);
RenderWindow createDefaultWindow(const std::string& name);
Text createDefaultHeaderText(Vector2u windowSize, const std::string& string);
RectangleShape createDefaultHeaderRect(RenderWindow window, int Width=300);

const int windowHeight = 500;
const int windowWidth = 500;
const int maxWindows = 7;
const int headerYPosition = 50;
const int headerHeight = 40;
bool active = false;
std::mt19937 rng; //random Funktion
int openWindows = 0;
int finishedWindows = 0;
Font font;
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

void initRandom() {
    std::random_device rd;
    rng.seed(rd()); 
}

int getRandInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max); // Gleichverteilung
    return dist(rng);
}

int gameOver()
{
    std::cout << "Game Over!" << std::endl;
    return 0;
}

int gameLoop()
{
    active = true;
    float difTime = 2;

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
    int window = getRandInt(0,0);
    if (window == 0) threads.push_back(std::thread(clicker));

    return 0;
}


#pragma endregion

#pragma region windows
RenderWindow createDefaultWindow(const std::string& name)
{
    RenderWindow window = RenderWindow(sf::VideoMode({windowWidth, windowHeight}), name);
    window.setFramerateLimit(144);
    int screenWidth = sf::VideoMode::getDesktopMode().size.x;
    int screenHeight = sf::VideoMode::getDesktopMode().size.y;
    
    int x = getRandInt(0, (screenWidth - windowWidth)); // std::rand() % (screenWidth - windowWidth);
    int y = getRandInt(0, (screenHeight - windowHeight)); // % (screenHeight - windowHeight);
    window.setPosition({x,y});

    return window;
}

RectangleShape createDefaultHeaderRect(const Vector2u windowSize, int Width=400)
{
    int Height = headerYPosition;
    RectangleShape rect(Vector2f(Width, Height));
    rect.setFillColor(Color::White);
    float x = (windowSize.x / 2) - (Width / 2);
    float y = headerHeight;
    rect.setPosition({x, y});

    return rect;
}

Text createDefaultHeaderText(Vector2u windowSize, const std::string& string)
{
    const int textSize = 24;
    Text text(font, string, textSize);
    text.setFillColor(Color::Black);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2,textBounds.size.y / 2});
    float x = (windowSize.x / 2);
    float y = headerYPosition + (headerHeight / 2) - (textSize / 2);
    text.setPosition({x, y});

    return text;
}

int clicker() 
{
    RenderWindow window = createDefaultWindow("Clicker");
    RectangleShape headerRect = createDefaultHeaderRect(window.getSize());
    Text headerText = createDefaultHeaderText(window.getSize(), "Click on the blue rect!");
    
    int rectAmount = getRandInt(10, 30);
    std::vector<Button> wrongButtons;

    // Liste von Farben
    std::vector<Color> colors = {
        Color::Color(33,133,346,255), 
        Color::Color(224,151,16,255), 
        Color::Color(224,92,16,255),
        Color::Color(0,184,70,255),
        Color::Color(184,0,184,255),
        Color::Color(125,0,184,255),
    };

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

#pragma endregion


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

            window.display();
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
    return 0;
}