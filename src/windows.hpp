//windows.hpp
#ifndef WINDOWS_HPP
#define WINDOWS_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "cwindow.hpp"


using namespace sf;

class Clicker : public cWindow {
protected:

public:
    Clicker();
    void start();
};

class BoxCounter : public cWindow {
    protected:
    
    public:
        BoxCounter();
        void start();
};
    
class Painter : public cWindow {
    protected:
    
    public:
        Painter();
        void start();
};
            
#endif