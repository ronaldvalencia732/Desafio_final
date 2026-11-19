#include "nivel.h"

nivel::nivel(QObject *parent) : QObject(parent) {
    cargarNivel(1);
}

void nivel::cargarNivel(int num) {
    numeroNivel = num;

    if (numeroNivel == 1) {
        // Cuartos de Final Libertadores 2016: Nacional 3 - 1 Rosario Central
        nombreFase = "NIVEL 1: CUARTOS DE FINAL (3-1)";
        rivalNombre = "ROSARIO";
        golesObjetivo = 3;
        tiempoLimite = 45;
        cantidadDefensores = 1;
        multiplicadorVelocidadRival = 1.0f;
        rutaFondo = ":/spritesJuego/cancha.jpeg";
    }
    else if (numeroNivel == 2) {
        // Semifinal Libertadores 2016: Nacional 2 - 1 Sao Paulo
        nombreFase = "NIVEL 2: SEMIFINAL (2-1)";
        rivalNombre = "SAO PAULO";
        golesObjetivo = 2;
        tiempoLimite = 50;
        cantidadDefensores = 2;
        multiplicadorVelocidadRival = 1.45f;
        rutaFondo = ":/spritesJuego/cancha.jpeg";
    }
    else {
        // Gran Final Libertadores 2016: Nacional 1 - 0 Ind. del Valle
        nombreFase = "NIVEL 3: GRAN FINAL (1-0)";
        rivalNombre = "IND. VALLE";
        golesObjetivo = 1;
        tiempoLimite = 60;
        cantidadDefensores = 3;
        multiplicadorVelocidadRival = 2.3f;
        rutaFondo = ":/spritesJuego/fondo_nivel1.jpeg";
    }
}