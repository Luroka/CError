//windows.hpp
#ifndef WINDOWS_HPP
#define WINDOWS_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "cwindow.hpp"
#include "button.hpp"


using namespace sf;

class Clicker : public cWindow {
    protected:
        RenderWindow window;
        RectangleShape headerRect;
        Text headerText;
        std::vector<Button> wrongButtons;
        Button rightButton;
    public:
        Clicker();
        void Clicker::createButtonRects(int rectWidth, int rectHeight, int minAmount, int maxAmount);
        void Clicker::draw();
};

class BoxCounter : public cWindow {
    protected:
        int counter = 0;
        int rectAmount = 0;
        const int textSize = 24;
        RenderWindow window;
        RectangleShape headerRect;
        Text headerText;
        std::vector<RectangleShape> rects;;
        Text numberText;
        Button moreButton;
        Button lessButton;
        Button confirmButton;
    public:
        BoxCounter();
        void resetWindow(int minRects);
        void BoxCounter::createRects(float rectWidth, int rectHeight, int rectAmount);
        void BoxCounter::draw();
};
    
class Painter : public cWindow {
    protected:
        const int textSize = 24;
        RenderWindow window;
        RectangleShape headerRect;
        Text headerText;
        std::vector<RectangleShape> rects;
    public:
        Painter();
        void Painter::createRects(float rectWidth, int rectHeight, int rectAmount);
        void Painter::draw();
};

class FillUp : public cWindow {
    protected:
        const int textSize = 24;
        int height = 0;
        float rectheight = 50;
        RenderWindow window;
        RectangleShape headerRect;
        Text headerText;
        Button fillButton;
        std::vector<RectangleShape> rects;
    public:
        FillUp();
        void FillUp::up();
        void FillUp::draw();
};
            
#endif