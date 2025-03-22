//window.hpp
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <random>
#include <iostream>

using namespace sf;

class cWindow {
protected:
    int windowHeight;
    int windowWidth;
    int headerYPosition;
    int headerHeight;
    std::mt19937 rng;
    Font font;
    // Liste von Farben
    std::vector<Color> colors;

public:
    cWindow();
    virtual ~cWindow() = default;
    void initRandom();
    RenderWindow createDefaultWindow(const std::string& name);
    RectangleShape createDefaultHeaderRect(const Vector2u windowSize, int Width=400);
    Text createDefaultHeaderText(Vector2u windowSize, const std::string& string);
};

#endif