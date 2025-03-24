//cwindow.cpp
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <thread>
#include <random>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "cwindow.hpp"
#include "button.hpp"
#include "globals.hpp"
using namespace sf;

cWindow::cWindow() {
    if (!font.openFromFile("./assets/RobotoMono-Regular.ttf")) std::cerr << "Cannot load font" << std::endl;// Fenster Grundfunktionen

    windowHeight = 500;
    windowWidth = 500;
    headerYPosition = 50;
    headerHeight = 40;
    // Liste von Farben
    colors = {
        Color::Color(33,133,246,255), 
        Color::Color(224,151,16,255), 
        Color::Color(224,92,16,255),
        Color::Color(0,184,70,255),
        Color::Color(184,0,184,255),
        Color::Color(125,0,184,255),
    };
};

RenderWindow cWindow::createDefaultWindow(const std::string& name)
{
    RenderWindow window = RenderWindow(VideoMode({500, 500}), name);

    window.setFramerateLimit(144);
    int screenWidth = VideoMode::getDesktopMode().size.x;
    int screenHeight = VideoMode::getDesktopMode().size.y;
    
    int x = getRandInt(0, (screenWidth - windowWidth));
    int y = getRandInt(0, (screenHeight - windowHeight - 100));
    window.setPosition({x,y});

    /*
    #ifdef _WIN32
        HWND hwnd = window.getNativeHandle();
        // Fenster im minimierten Zustand starten
        ShowWindow(hwnd, SW_SHOWMINNOACTIVE); 
    #endif
    */

    // Kurze Verzögerung und Fokus zurückgeben
    window.setVisible(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    window.setVisible(true);
    window.requestFocus();

    return window;
}

RectangleShape cWindow::createDefaultHeaderRect(const Vector2u windowSize, int Width)
{
    int Height = headerYPosition;
    RectangleShape rect(Vector2f(Width, Height));
    rect.setFillColor(Color::White);
    float x = (windowSize.x / 2) - (Width / 2);
    float y = headerHeight;
    rect.setPosition({x, y});

    return rect;
}

Text cWindow::createDefaultHeaderText(Vector2u windowSize, const std::string& string)
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