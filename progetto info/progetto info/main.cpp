#include <iostream>
using namespace std;

#define MINRGB 0
#define MAXRGB 255

// Variabili globali per il colore
int rosso = 0;
int verde = 0;
int blu = 0;

// Variabili globali per le coordinate del blocco
int x = 0;
int y = 0;

// Funzioni per manipolare i valori del colore
void inizializzaColore(int r, int g, int b) {
    rosso = r;
    verde = g;
    blu = b;
}
int getRed() {
    return rosso;
}

int getGreen() {
    return verde;
}

int getBlue() {
    return blu;
}

void setRed(int r) {
    rosso = r;
}

void setGreen(int g) {
    verde = g;
}

void setBlue(int b) {
    blu = b;
}
