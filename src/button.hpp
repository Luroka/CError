#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button {
private:
    sf::RectangleShape shape;
    bool wasPressedLastFrame = false; // Flag zur Erkennung der vorherigen Mauszustände
    bool isActive = true;
    bool isVisible = true; 
    sf::Text text;
    static sf::Font font;

public:
    Button(float x, float y, float width, float height, const std::string& label, sf::Color color = sf::Color::Blue, sf::Color textColor = sf::Color::White, int Textsize=24);
    void draw(sf::RenderWindow& window);
    bool isTouched(sf::RenderWindow& window);
    bool isClicked(sf::RenderWindow& window);
    bool clickDown(sf::RenderWindow& window);
    void setActive(bool active);
    void setVisibility(bool Visibility);
};

#endif