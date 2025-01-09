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
#define NUMEROFORME 8            // Numero totale di tetromini disponibili (7 diverse forme)
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
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // T
	{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {1, 0, 0, 1}}  //T al contrario
};

// Variabili globali
int TetrominoOccorrente[4][4];  // La matrice del tetromino attualmente in gioco
int RigaOccorrente = 0;         // La riga corrente in cui si trova il tetromino (inizialmente 0)
int ColonnaOccorrente = 3;      // La colonna corrente in cui si trova il tetromino (inizialmente 3, al centro)
int contatore = 0;              // Il punteggio del gioco, che aumenta con la pulizia delle linee
int LineeCompletate = 0;        // Il numero di linee che sono state completate

void run() {
    InizializzaGioco();
    GeneraTetromino();
    UseDoubleBuffering(true);

    while (true) {

        if (ControllaGameOver()) {
            // Disegna il messaggio "Game Over"
            Clear();
            string gameOverStr = "Game Over!";
            DrawString((IMM2D_WIDTH / 2) - 5, (IMM2D_HEIGHT / 2) - 55, gameOverStr.c_str(), "Arial", 40, Red, true);

            Present();

            while (true) {
                int tasto = LastKey();
                if (tasto == 27) { // Se premi ESC, esce dal gioco
                    return;
                }
            }
        }

        Clear(); // Pulizia dello schermo
        DisegnaGriglia();
        DisegnaTetromino();

        string punteggioStr = "Punteggio: " + to_string(contatore);
        DrawString(95, 10, punteggioStr.c_str(), "Arial", 20, White, true);

        string lineeStr = "Linee: " + to_string(LineeCompletate);
        DrawString(67, 40, lineeStr.c_str(), "Arial", 20, White, true);

        Present();

        int tasto = LastKey();
        if (tasto != 0) {
            tasto = toupper(tasto); // Converte i tasti in maiuscolo per compatibilità
        }
        if (tasto == 27) break; // Esce con il tasto ESC
        else if (tasto == 'A' || tasto == 'D' || tasto == 'S') {
            MuoviTetromino(tasto); // Passa direttamente il carattere alla funzione
        }

        // Movimento automatico verso il basso
        MuoviTetromino('S');
        Sleep(110); // Velocità di caduta

    }
}

// Funzione per inizializzare il campo di gioco (tutte le celle inizialmente vuote)
void InizializzaGioco() {
    for (int r = 0; r < RIGHE; r++) {
        for (int c = 0; c < COLONNE; c++) {
            MatriceGioco[r][c] = 0;  // Imposta ogni cella della matrice a 0 (vuota)
        }
    }
}

// Funzione che copia un tetromino (forma) dalla matrice FormeTetromino alla matrice TetrominoOccorrente
void CopiaTetromino(int tetromino[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            TetrominoOccorrente[r][c] = tetromino[r][c];  // Copia ogni valore dalla forma
        }
    }
}

// Funzione per generare un tetromino casuale (selezionando un indice casuale tra 0 e NUMEROFORME-1)
void GeneraTetromino() {
    int index = RandomInt(0, NUMEROFORME - 1);  // Genera un numero casuale per scegliere un tetromino
    CopiaTetromino(FormeTetromino[index]);       // Copia il tetromino selezionato nella matrice corrente
    RigaOccorrente = 0;                          // Posiziona il tetromino all'inizio del campo (in alto)
    ColonnaOccorrente = 3;                       // Posiziona il tetromino nel centro del campo (colonna 3)
}

// Funzione che verifica se il movimento del tetromino è valido
// Un movimento è valido se il tetromino non esce dal campo o se non collida con altri blocchi
bool ControlloMovimento(int NuovaRighe, int NuovaColonna) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrominoOccorrente[r][c]) {  // Se la cella del tetromino è occupata
                int RigheGioco = NuovaRighe + r;
                int ColonneGioco = NuovaColonna + c;
                // Verifica se il tetromino esce dai limiti o se la cella è già occupata
                if (RigheGioco < 0 || RigheGioco >= RIGHE || ColonneGioco < 0 || ColonneGioco >= COLONNE || MatriceGioco[RigheGioco][ColonneGioco]) {
                    return false;  // Il movimento non è valido
                }
            }
        }
    }
    return true;  // Il movimento è valido
}

// Funzione che piazza il tetromino corrente nel campo di gioco (matrice MatriceGioco)
void PiazzaTetromino() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrominoOccorrente[r][c]) {
                MatriceGioco[RigaOccorrente + r][ColonnaOccorrente + c] = 1;  // Aggiunge il blocco alla matrice
            }
        }
    }
}

// Funzione che controlla e pulisce le linee complete nel campo di gioco
// Sposta le righe sopra verso il basso quando una riga è completa
void PulisciLinee() {
    for (int r = RIGHE - 1; r >= 0; r--) {  // Scansiona le righe dal basso verso l'alto
        bool RigaPiena = true;
        for (int c = 0; c < COLONNE; c++) {
            if (!MatriceGioco[r][c]) {  // Se c'è una cella vuota nella riga
                RigaPiena = false;  // La riga non è piena
                break;  // Interrompe la verifica della riga
            }
        }
        if (RigaPiena) {  // Se la riga è piena
            for (int rr = r; rr > 0; rr--) {
                for (int cc = 0; cc < COLONNE; cc++) {
                    MatriceGioco[rr][cc] = MatriceGioco[rr - 1][cc];  // Sposta le righe sopra verso il basso
                }
            }
            for (int cc = 0; cc < COLONNE; cc++) {
                MatriceGioco[0][cc] = 0;  // Imposta la prima riga come vuota
            }
            LineeCompletate++;  // Aumenta il contatore delle linee completate
            contatore += 100;   // Aumenta il punteggio di 100 per ogni riga completata
            r++;  // Riprova sulla stessa riga per gestire eventuali linee multiple completate
        }
    }
}

// Funzione che disegna la griglia di gioco, rappresentando ogni cella come un rettangolo
void DisegnaGriglia() {
    int offsetX = (IMM2D_WIDTH - (COLONNE * DIMENSIONE_CELLA)) / 2;  // Calcola il margine orizzontale per centrare la griglia
    int offsetY = (IMM2D_HEIGHT - (RIGHE * DIMENSIONE_CELLA)) / 2;    // Calcola il margine verticale per centrare la griglia

    for (int r = 0; r < RIGHE; r++) {
        for (int c = 0; c < COLONNE; c++) {
            int x = offsetX + c * DIMENSIONE_CELLA;  // Calcola la posizione X della cella
            int y = offsetY + r * DIMENSIONE_CELLA;  // Calcola la posizione Y della cella

            if (MatriceGioco[r][c]) {
                DrawRectangle(x, y, DIMENSIONE_CELLA, DIMENSIONE_CELLA, Red, true);  // Disegna la cella con colore rosso se occupata
            }
            else {
                DrawRectangle(x, y, DIMENSIONE_CELLA, DIMENSIONE_CELLA, LightGray, false);  // Cella vuota, colore grigio chiaro
            }
        }
    }
}

// Funzione che disegna il tetromino corrente nella sua posizione sulla griglia
void DisegnaTetromino() {
    int offsetX = (IMM2D_WIDTH - (COLONNE * DIMENSIONE_CELLA)) / 2;
    int offsetY = (IMM2D_HEIGHT - (RIGHE * DIMENSIONE_CELLA)) / 2;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrominoOccorrente[r][c]) {
                int x = offsetX + (ColonnaOccorrente + c) * DIMENSIONE_CELLA;  // Calcola la posizione X del tetromino
                int y = offsetY + (RigaOccorrente + r) * DIMENSIONE_CELLA;  // Calcola la posizione Y del tetromino
                DrawRectangle(x, y, DIMENSIONE_CELLA, DIMENSIONE_CELLA, Blue, true);  // Disegna il blocco del tetromino in blu
            }
        }
    }
}


// Funzione che muove il tetromino nella direzione specificata (S = giù, A = sinistra, D = destra)
void MuoviTetromino(char Direzione) {
    int NuovaRiga = RigaOccorrente;
    int NuovaColonna = ColonnaOccorrente;

    if (Direzione == 'S') {
        NuovaRiga++; // Muove il tetromino verso il basso
    }
    else if (Direzione == 'A') {
        NuovaColonna--; // Muove il tetromino a sinistra
    }
    else if (Direzione == 'D') {
        NuovaColonna++; // Muove il tetromino a destra
    }

    // Verifica se il movimento è valido
    if (ControlloMovimento(NuovaRiga, NuovaColonna)) {
        RigaOccorrente = NuovaRiga;  // Aggiorna la posizione del tetromino
        ColonnaOccorrente = NuovaColonna;
    }
    else if (Direzione == 'S') {  // Se il movimento verso il basso non è valido (è bloccato)
        PiazzaTetromino();  // Piazza il tetromino nel campo di gioco
        PulisciLinee();     // Pulisce le linee completate
        GeneraTetromino();  // Genera un nuovo tetromino
    }
}

// Funzione che verifica se il gioco è finito
// Il gioco finisce se la prima riga è già occupata da blocchi
bool ControllaGameOver() {
    for (int c = 0; c < COLONNE; c++) {
        if (MatriceGioco[0][c] != 0) {
            return true;  // Se una cella nella prima riga è occupata, il gioco è finito
        }
    }
    return false;  // Il gioco non è finito
}








