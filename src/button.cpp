// Button.cpp
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <iostream>
#include <string>

sf::Font Button::font;

Button::Button()
: text(font, "", 24)
{
    
}

Button::Button(float x, float y, float width, float height, const std::string& label, sf::Color color, sf::Color textColor, int Textsize) 
    : text(font, label, Textsize)
{
    // Form des Buttons
    shape.setPosition({x, y});
    shape.setOrigin({width / 2, height / 2});
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(color);

    // Text im Button
    static bool isFontLoaded = false;
    if (!isFontLoaded) {
        if (!font.openFromFile("./assets/RobotoMono-Regular.ttf")) {
            std::cerr << "Cannot load font" << std::endl;
        }
        isFontLoaded = true;
    }
    text.setFont(font);
    text.setFillColor(textColor);

    // Text zentrieren
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2,textBounds.size.y / 2});
    text.setPosition({x, y});
}

void Button::draw(sf::RenderWindow& window) 
{
    if (!isVisible) return;
    window.draw(shape);
    window.draw(text);
}

//true wenn die Maus auf dem Button ist
bool Button::isTouched(sf::RenderWindow& window) 
{
    if (!isActive) return false;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window); 
    return (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)));
}

//true wenn die Maus auf dem Button ist und die linke Maustraste gedrückt ist
bool Button::isClicked(sf::RenderWindow& window) 
{
    if (!isActive) return false;
    return (isTouched(window) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
}

//einmalig true wenn der Button mit der Linken Maus angeklickt wurde
bool Button::clickDown(sf::RenderWindow& window) 
{
    if (!isActive) return false;

    bool mousePressedLeft = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    if (isClicked(window) && !wasPressedLastFrame)
    {
        wasPressedLastFrame = true;
        return true;
    }

    wasPressedLastFrame = mousePressedLeft;

    return false;
}

void Button::setActive(bool active)
{
    isActive = active;
}

void Button::setVisibility(bool Visibility) 
{
    isVisible = Visibility;
}