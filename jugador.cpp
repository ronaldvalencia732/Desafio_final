#include "jugador.h"
#include <QPixmap>

jugador::jugador(QString nombreJugador, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), nombre(nombreJugador) {

    posicionX = 300;
    posicionY = 200;
    velocidad = 5.0f;
    caracteristicaActiva = "Normal";
    resistencia = 100.0f;
    currentFrame = 0;

    // Carga de la imagen sin encabezados desde recursos
    spriteSheet.load(":/spritesJuego/jugador_verdolaga.jpeg");
    recortarSprite(0, 0); // Frame inicial (Idle)
    setPos(posicionX, posicionY);
}

void jugador::recortarSprite(int row, int col) {
    if (spriteSheet.isNull()) return;

    // Cálculo dinámico de celdas (8 columnas x 5 filas)
    int frameWidth = spriteSheet.width() / 8;
    int frameHeight = spriteSheet.height() / 5;

    QPixmap cropped = spriteSheet.copy(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
    setPixmap(cropped.scaled(55, 55, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void jugador::mover(float dx, float dy) {
    posicionX += dx * velocidad;
    posicionY += dy * velocidad;
    setPos(posicionX, posicionY);

    if (dx != 0 || dy != 0) {
        currentFrame = (currentFrame + 1) % 6; // Iteración de frames de carrera
        recortarSprite(1, currentFrame);       // Fila 1 = Running
    } else {
        recortarSprite(0, 0);                  // Fila 0 = Idle
    }
}

void jugador::activarHabilidadEspecial(QString habilidad) {
    caracteristicaActiva = habilidad;
    if (habilidad == "Veloz") {
        velocidad = 9.0f;
        recortarSprite(4, 0); // Fila 4 = Modo Veloz
    } else if (habilidad == "Arriesgado") {
        recortarSprite(3, 0); // Fila 3 = Modo Arriesgado (Power Kick)
    }
}