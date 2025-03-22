// windows.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include "windows.hpp"
#include "button.hpp"
#include "globals.hpp"

using namespace sf;

#pragma region clicker
Clicker::Clicker() : cWindow(),
headerText(font, "", 24)
{
    //if (!font.openFromFile("./assets/RobotoMono-Regular.ttf")) std::cerr << "Cannot load font" << std::endl;

    window = createDefaultWindow("Clicker");
    headerRect = createDefaultHeaderRect(window.getSize());
    headerText = createDefaultHeaderText(window.getSize(), "Click on the blue rect!");
    
    int rectHeight = 40;
    int rectWidth = 40;
    createButtonRects(rectWidth, rectHeight, 5, 15);
    std::cout << wrongButtons.size();

    //richtigen Button setzen 
    float x = getRandInt(0, (windowWidth - rectWidth));
    float y = getRandInt(90, (windowHeight - rectHeight));
    rightButton = Button(x,y, rectWidth, rectHeight, "", Color::Blue);

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

            draw();
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
}

void Clicker::createButtonRects(int rectWidth, int rectHeight, int minAmount, int maxAmount) 
{
    //Rechtecke erstellen und random positionieren
    int rectAmount = getRandInt(minAmount, maxAmount);
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        //falsche Button setzen
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight));
        Button button(x,y, rectWidth, rectHeight, "", colors.at(i%colorSize));
        wrongButtons.push_back(button);
    }
}

void Clicker::draw()
{
    window.clear();
    for (Button& b : wrongButtons) {
        b.draw(window);
    }
    rightButton.draw(window);
    window.draw(headerRect);
    window.draw(headerText);
    window.display();
}


#pragma endregion

#pragma region boxCounter
BoxCounter::BoxCounter() : cWindow(),
headerText(font, "", 24),
numberText(font, "0", 24)
{
    window = createDefaultWindow("box count");
    headerRect = createDefaultHeaderRect(window.getSize(), 300);
    headerText = createDefaultHeaderText(window.getSize(), "count all boxes");

    //Zähler-Text erstellen
    numberText.setFillColor(Color::White);
    FloatRect textBounds = numberText.getLocalBounds();
    numberText.setOrigin({textBounds.size.x / 2,textBounds.size.y / 2});
    float x = (window.getSize().x / 2);
    float y = window.getSize().y - 50;
    numberText.setPosition({x, y});

    //Rechtecke erstellen und random positionieren
    float rectHeight = 40;
    float rectWidth = 40;
    int rectAmount = getRandInt(5, 10);
    createRects(rectWidth, rectHeight, rectAmount);


    int couter = 0;

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

            numberText.setString(std::to_string(couter));

            if (couter == rectAmount && Keyboard::isKeyPressed(Keyboard::Key::Enter))
            {}

            draw();
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
}

void BoxCounter::createRects(float rectWidth, int rectHeight, int rectAmount) 
{
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight - textSize));
        RectangleShape rect(Vector2f(rectWidth, rectHeight));
        //rect.setOrigin({rectWidth / 2, rectHeight / 2});
        rect.setPosition({x,y});
        rect.setFillColor(colors.at(i%colorSize));
        rects.push_back(rect);
    }
}

void BoxCounter::draw()
{
    window.clear();
    window.draw(numberText);
    for (RectangleShape& r : rects) {
        window.draw(r);
    }  
    window.draw(headerRect);
    window.draw(headerText);
    window.display();
}
#pragma endregion

#pragma region painter
Painter::Painter() : cWindow(),
headerText(font, "", 24)
{
    window = createDefaultWindow("painter");
    headerRect = createDefaultHeaderRect(window.getSize());
    headerText = createDefaultHeaderText(window.getSize(), "use the arrows to color all rects");
    
    
    int rectAmount = getRandInt(3, 10);

    //Rechtecke erstellen und random positionieren
    float rectHeight = 40;
    float rectWidth = 40;
    int colorSize = colors.size();
    std::cout << "CSize" << colorSize << std::endl;
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
                return;
            }

            draw();
            window.draw(brush);
        }
    }

    openWindows -= 1;
    finishedWindows += 1;
}

void Painter::createRects(float rectWidth, int rectHeight, int rectAmount) 
{
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight - textSize));
        RectangleShape rect(Vector2f(rectWidth, rectHeight));
        //rect.setOrigin({rectWidth / 2, rectHeight / 2});
        rect.setPosition({x,y});
        rect.setFillColor(colors.at(i%colorSize));
        rects.push_back(rect);
    }
}

void Painter::draw()
{
    window.draw(headerRect);
    window.draw(headerText);
    for (RectangleShape& r : rects) {
        window.draw(r);
    }
    window.display();
}
#pragma endregion
