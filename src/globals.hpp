#ifndef GLOBALS_HPP
#define GLOBALS_HPP

extern bool active;  // Deklaration der globalen Variable
extern int openWindows;
extern int finishedWindows;
extern std::mt19937 rng;
extern void initRandom();

extern int getRandInt(int min, int max);

#endif // GLOBALS_H
