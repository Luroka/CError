#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <SFML/Window.hpp>
#include <Windows.h> // Für SetWindowPos()
#include <random>
#include <ctime>   // Für time()
#include "Button.hpp"

using namespace sf;


int gameOver();
int gameLoop();
int clicker();
int boxCouter();
int painter(); 
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

// Liste von Farben
static std::vector<Color> colors = {
    Color::Color(33,133,346,255), 
    Color::Color(224,151,16,255), 
    Color::Color(224,92,16,255),
    Color::Color(0,184,70,255),
    Color::Color(184,0,184,255),
    Color::Color(125,0,184,255),
};

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
    if (window == 1) threads.push_back(std::thread(boxCouter));
    if (window == 2) threads.push_back(std::thread(painter));

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

    WindowHandle wHandle = window.getNativeHandle();
    //window.setMaximumSize({windowWidth, windowHeight});
    //window.setMinimumSize({windowWidth, windowHeight});
    

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
    FloatRect textBounds = text.getLocalBounds();
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

int painter() 
{
    RenderWindow window = createDefaultWindow("painter");
    RectangleShape headerRect = createDefaultHeaderRect(window.getSize());
    Text headerText = createDefaultHeaderText(window.getSize(), "use the arrows to color all rects");
    
    
    int rectAmount = getRandInt(3, 10);
    std::vector<RectangleShape> rects;

    //Rechtecke erstellen und random positionieren
    float rectHeight = 40;
    float rectWidth = 40;
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight));
        RectangleShape rect(Vector2f(rectWidth, rectHeight));
        rect.setOrigin({rectWidth / 2, rectHeight / 2});
        rect.setPosition({x,y});
        rect.setFillColor(colors.at(i%colorSize));
        rects.push_back(rect);
    }

    Color brushColor = Color::Color(87, 81, 250);
    

    //auf freepik.com von Arslan Haider
    sf::Texture texture;
    if (!texture.loadFromFile("./assets/brush.png", false, sf::IntRect({0, 0}, {256, 256})))
    {
        std::cout << "Error beim laden von brush.png.";
    }

    
    Sprite brush(texture);
    brush.setTexture(texture);
    brush.setOrigin({128.f, 128.f});
    brush.setScale({0.2f, 0.2f});
    brush.setPosition({250.f, 500.f-128.f});
    
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

            //darstellen
            window.clear();
            
            float speed = 5;
            if (Keyboard::isKeyPressed(Keyboard::Key::Up))
            {
                brush.move({0, -speed});
            }
            else if (Keyboard::isKeyPressed(Keyboard::Key::Down))
            {
                brush.move({0, speed});
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                brush.move({speed, 0});
            }
            else if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                brush.move({-speed, 0});
            }

            //Rects färben falls der Pinsel sie anmalt
            for (RectangleShape& r : rects) {
                if (r.getFillColor() != brushColor)
                {
                    if (brush.getGlobalBounds().findIntersection(r.getGlobalBounds()))
                    {
                        r.setFillColor(brushColor);
                        counter += 1;
                    }
                }
            }

            if (counter >= rectAmount)
            {
                window.close();
                return 0;
            }

            window.draw(headerRect);
            window.draw(headerText);
            for (RectangleShape& r : rects) {
                window.draw(r);
            }
            window.draw(brush);
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

            window.draw(headerRect);
            window.draw(headerText);
            window.display();
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
    return 0;
}