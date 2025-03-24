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
    //Grundlegendes Fenster erstellen
    window = createDefaultWindow("Clicker");
    headerRect = createDefaultHeaderRect(window.getSize());
    headerText = createDefaultHeaderText(window.getSize(), "Click on the blue rect!");
    
    //Falesche Buttons erstellen
    int rectHeight = 40;
    int rectWidth = 40;
    createButtonRects(rectWidth, rectHeight, 5, 15);

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

        //Abbruchbedingung
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
                finishedWindows += 1;
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
    //Grundlegendes Fenster erstellen
    window = createDefaultWindow("box count");
    headerRect = createDefaultHeaderRect(window.getSize(), 300);
    headerText = createDefaultHeaderText(window.getSize(), "count all boxes");

    //rects erstellen
    resetWindow(0);

    //Zähler-Text erstellen
    numberText.setFillColor(Color::White);
    FloatRect textBounds = numberText.getLocalBounds();
    numberText.setOrigin({textBounds.size.x / 2,textBounds.size.y / 2});
    float windowMid = (window.getSize().x / 2);
    float y = window.getSize().y - 70;
    numberText.setPosition({windowMid, y});

    //Buttons erstellen
    moreButton = Button(windowMid + 60, y, 30, 30, ">", Color::Blue);
    lessButton = Button(windowMid - 60, y, 30, 30, "<", Color::Blue);
    confirmButton = Button(windowMid, y + 50, 100, 30, "confirm", Color::Blue);


    bool firstFrame = true; //Damit das Fenster einmal am Anfang lädt auch wenn es nicht im Fokus ist
    while (window.isOpen())
    {
        //Event loop, damit es nicht zu Fehlern kommt
        while (const std::optional event = window.pollEvent())
        {        }
        
        //Abbruchbedingung
        if (active == false) 
        {
            window.close();
        }

        if (window.hasFocus() || firstFrame)
        {
            firstFrame = false;

            numberText.setString(std::to_string(counter));

            //mehr
            if (moreButton.clickDown(window)) counter++;

            //weniger
            if (lessButton.clickDown(window))
            {
                counter--;
                if (counter < 0) counter = 0;
            }

            //confirm
            if (confirmButton.clickDown(window))
            {
                if (counter == rectAmount)
                {
                    window.close();
                    finishedWindows += 1;
                }
                else
                {
                    resetWindow(3);
                }
            }

            draw();
        }
    }

    openWindows -= 1;
}

void BoxCounter::resetWindow(int minRects)
{
    //Rechtecke erstellen und random positionieren
    float rectHeight = 40;
    float rectWidth = 40;
    rectAmount = getRandInt(minRects, getRandInt(6,8));
    createRects(rectWidth, rectHeight, rectAmount);
}

void BoxCounter::createRects(float rectWidth, int rectHeight, int rectAmount) 
{
    rects.clear();
    int colorSize = colors.size();
    for (int i = 0; i < rectAmount; ++i) {
        float x = getRandInt(0, (windowWidth - rectWidth));
        float y = getRandInt(90, (windowHeight - rectHeight - textSize));
        RectangleShape rect(Vector2f(rectWidth, rectHeight));
        rect.setPosition({x,y});
        rect.setFillColor(colors.at(i%colorSize));
        rects.push_back(rect);
    }
}

void BoxCounter::draw()
{
    window.clear();
    for (RectangleShape& r : rects) {
        window.draw(r);
    }
    window.draw(numberText);
    window.draw(headerRect);
    window.draw(headerText);
    lessButton.draw(window);
    moreButton.draw(window);
    confirmButton.draw(window);

    window.display();
}
#pragma endregion

#pragma region painter
Painter::Painter() : cWindow(),
headerText(font, "", 24)
{
    //Grundlegendes Fenster erstellen
    window = createDefaultWindow("painter");
    headerRect = createDefaultHeaderRect(window.getSize());
    headerText = createDefaultHeaderText(window.getSize(), "color all rects purple");
    
    //Rechtecke erstellen und random positionieren
    float rectHeight = 40;
    float rectWidth = 40;
    int rectAmount = getRandInt(3, 10);
    createRects(rectWidth, rectHeight, rectAmount);

    Color brushColor = Color::Color(87, 81, 250);
    
    //auf freepik.com von Arslan Haider
    //Pinsel erstellen
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
        
        //Abbruchbedingung
        if (active == false) 
        {
            window.close();
        }

        if (window.hasFocus() || firstFrame)
        {

            firstFrame = false;

            //Maus mit dem Pinsel verfolgen
            sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Mausposition relativ zum Fenster
            brush.setPosition({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});

            //Window erfolgreich abgeschlossen
            if (counter >= rectAmount)
            {
                sleep(milliseconds(10));
                window.close();
                finishedWindows += 1;
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
            
            //darstellen
            window.clear();
            draw();
            window.draw(brush);
            window.display();
        }
    }

    openWindows -= 1;
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
}
#pragma endregion

#pragma region fillUp

FillUp::FillUp() : cWindow(),
headerText(font, "", 24)
{
    //Grundlegendes Fenster erstellen
    window = createDefaultWindow("fill up");
    headerRect = createDefaultHeaderRect(window.getSize());
    headerText = createDefaultHeaderText(window.getSize(), "fill the window");
    
    //Variablen setzen
    height = 0;
    rectheight = getRandInt(40, 150);
    int winHeight = window.getSize().y;
    float windowMid = window.getSize().x / 2;

    //Button erstellen
    float y = window.getSize().y - 50;
    fillButton = Button(windowMid, y, 50, 30, "fill", Color::Blue);

    bool firstFrame = true; //Damit das Fenster einmal am Anfang lädt auch wenn es nicht im Fokus ist
    while (window.isOpen())
    {
        //Event loop, damit es nicht zu Fehlern kommt
        while (const std::optional event = window.pollEvent())
        {        }
        
        //Abbruchbedingung
        if (active == false) 
        {
            window.close();
        }

        if (window.hasFocus() || firstFrame)
        {
            firstFrame = false;

            //Window erflgreich abgeschlossen
            if (winHeight <= height + rectheight)
            {
                window.close();
                finishedWindows += 1;
            }

            //Bildschirm weiter ausfüllen
            if (fillButton.clickDown(window))
            {
                up();
            }

            draw();
        }
    }

    openWindows -= 1;
    return;
}

void FillUp::up()
{
    RectangleShape rect(Vector2f(window.getSize().x, rectheight));
    height += rectheight;
    rect.setPosition({static_cast<float>(0), static_cast<float>(window.getSize().y - height)});
    rect.setFillColor(colors.at(getRandInt(0, colors.size() - 1)));
    rects.push_back(rect);
}

void FillUp::draw()
{
    window.clear();
    window.draw(headerRect);
    window.draw(headerText);
    for (RectangleShape& r : rects) {
        window.draw(r);
    }
    fillButton.draw(window);
    window.display();
}
#pragma endregion
