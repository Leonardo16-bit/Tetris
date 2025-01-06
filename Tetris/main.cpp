#define IMM2D_WIDTH  820        // Larghezza della finestra del gioco
#define IMM2D_HEIGHT 600         // Altezza della finestra del gioco
#define IMM2D_SCALE 1            // Scala per la visualizzazione degli oggetti grafici (impostato su 1, quindi nessuna scala)
#define IMM2D_IMPLEMENTATION     // Implementazione della libreria di Immediate2D per il rendering grafico

#include <cstdlib>
#include "immediate2d.h"        // Include la libreria per il rendering grafico 2D
#include <string>
#include <Windows.h>             // Include la libreria di Windows per l'accesso alle API di sistema

// Costanti del gioco
#define COLONNE 10               // Numero di colonne nel campo di gioco (10)
#define RIGHE 15                 // Numero di righe nel campo di gioco (15)
#define NUMEROFORME 7            // Numero totale di tetromini disponibili (7 diverse forme)
#define VELOCITA 500             // Velocità di caduta dei tetromini in millisecondi (500ms)
#define DIMENSIONE_CELLA 40      // La dimensione di ciascuna cella (quadrato) della griglia, in pixel

using namespace std;

// Dichiarazione globale della matrice che rappresenta il campo di gioco
int MatriceGioco[RIGHE][COLONNE]; // La matrice contiene 0 per le celle vuote e 1 per quelle occupate

// Array che contiene le diverse forme dei tetromini in formato matrice 4x4
int FormeTetromino[NUMEROFORME][4][4] = {
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}, // I (colonna verticale)
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, // J
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, // L
    {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O (quadrato)
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // T
};

// Variabili globali
int TetrominoOccorrente[4][4];  // La matrice del tetromino attualmente in gioco
int RigaOccorrente = 0;         // La riga corrente in cui si trova il tetromino (inizialmente 0)
int ColonnaOccorrente = 3;      // La colonna corrente in cui si trova il tetromino (inizialmente 3, al centro)
int contatore = 0;              // Il punteggio del gioco, che aumenta con la pulizia delle linee
int LineeCompletate = 0;        // Il numero di linee che sono state completate

// Funzione per inizializzare il campo di gioco (tutte le celle inizialmente vuote)
void InizializzaGioco() {
    for (int r = 0; r < RIGHE; r++) {
        for (int c = 0; c < COLONNE; c++) {
            MatriceGioco[r][c] = 0;  // Imposta ogni cella della matrice a 0 (vuota)
        }
    }
}