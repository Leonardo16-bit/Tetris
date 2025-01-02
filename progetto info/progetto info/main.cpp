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
// Funzioni per manipolare i valori del blocco
void inizializzaBlocco(int posX, int posY, int r, int g, int b) {
    x = posX;
    y = posY;
    inizializzaColore(r, g, b);
}

int getX() {
    return x;
}

int getY() {
    return y;
}

void setX(int posX) {
    x = posX;
}

void setY(int posY) {
    y = posY;
}

void aggiungiBlocco(int dx, int dy) {
    x += dx;
    y += dy;
}

void sottraiBlocco(int dx, int dy) {
    x -= dx;
    y -= dy;
}

// Funzione principale
int main() {
    // Inizializza un blocco
    inizializzaBlocco(5, 10, 255, 0, 0); // Blocco rosso a posizione (5, 10)
    cout << "Posizione iniziale del blocco: (" << getX() << ", " << getY() << ")" << endl;
    cout << "Colore del blocco: R=" << getRed() << ", G=" << getGreen() << ", B=" << getBlue() << endl;

    // Modifica la posizione
    aggiungiBlocco(3, -2);
    cout << "Dopo aggiunta: (" << getX() << ", " << getY() << ")" << endl;

    // Cambia colore
    setGreen(255); // Cambia il colore a giallo
    cout << "Nuovo colore del blocco: R=" << getRed() << ", G=" << getGreen() << ", B=" << getBlue() << endl;

    return 0;
}
