#include "rival.h"
#include <QImage>
#include <QColor>
#include <cmath>

rival::rival(QString tipoRol, float x, float y, float limSup, float limInf, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), rol(tipoRol), posX(x), posY(y),
    limiteSuperior(limSup), limiteInferior(limInf) {

    currentFrame = 0;
    // Velocidades base más bajas para que el Nivel 1 sea fluido y accesible
    velocidadY = (rol == "Arquero") ? 1.6f : 1.2f;

    spriteSheet.load(":/spritesJuego/jugador_rival.jpeg");

    // Fila 3 = Arquero amarillo en guardia; Fila 5 = Jugador de campo rojo
    int filaInicial = (rol == "Arquero") ? 3 : 5;
    recortarSprite(filaInicial, 1);

    setPos(posX, posY);

    // Timer de movimiento autónomo (~30 FPS)
    timerIA = new QTimer(this);
    connect(timerIA, &QTimer::timeout, this, &rival::moverAutomatico);
    timerIA->start(33);
}

void rival::recortarSprite(int row, int col) {
    if (spriteSheet.isNull()) return;

    // La hoja de rivales tiene 8 columnas y 7 filas aprox.
    int frameWidth = spriteSheet.width() / 8;
    int frameHeight = spriteSheet.height() / 7;

    // Se resta 2px al alto para evitar recortar bordes de la fila inferior
    QPixmap cropped = spriteSheet.copy(col * frameWidth, row * frameHeight, frameWidth, frameHeight - 2);

    // Limpiar el fondo cuadriculado JPEG
    QImage img = cropped.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor color = img.pixelColor(x, y);
            if (color.red() > 190 && color.green() > 190 && color.blue() > 190 &&
                std::abs(color.red() - color.green()) < 15 &&
                std::abs(color.green() - color.blue()) < 15) {
                img.setPixelColor(x, y, Qt::transparent);
            }
        }
    }

    QPixmap transparente = QPixmap::fromImage(img);
    setPixmap(transparente.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void rival::moverAutomatico() {
    posY += velocidadY;

    if (posY <= limiteSuperior) {
        posY = limiteSuperior;
        velocidadY = std::abs(velocidadY);
    } else if (posY >= limiteInferior) {
        posY = limiteInferior;
        velocidadY = -std::abs(velocidadY);
    }

    setPos(posX, posY);

    // Animar pasos mientras patrulla
    int fila = (rol == "Arquero") ? 3 : 5;
    currentFrame = 1 + ((currentFrame + 1) % 5); // Columnas 1 a 5 (evitando la col 0 con etiquetas)
    recortarSprite(fila, currentFrame);
}