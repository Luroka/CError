#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <SFML/Graphics.hpp>
#include "globals.hpp"

bool active = true;  // Definition der globalen Variable
int openWindows = 0;
int finishedWindows = 0;
std::mt19937 rng;

void initRandom() {
    std::random_device rd;
    rng.seed(rd()); 
}

int getRandInt(int min, int max) {
    if (min > max) return min;
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}