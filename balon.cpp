#include "balon.h"
#include <QPainter>
#include <QPixmap>
#include <cmath>

balon::balon(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent) {
    posX = 400;
    posY = 300;
    velX = 0.0f;
    velY = 0.0f;
    friccion = 0.96f; // Desaceleración progresiva en el césped

    // Dibujar un balón de 18x18 px con fondo 100% transparente
    QPixmap pixmap(18, 18);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 1.5));
    painter.drawEllipse(1, 1, 16, 16);

    // Pentágono central clásico
    painter.setBrush(Qt::black);
    painter.drawEllipse(6, 6, 6, 6);
    painter.end();

    setPixmap(pixmap);
    setPos(posX, posY);

    // Timer interno para simular movimiento continuo a ~60 FPS
    timerFisica = new QTimer(this);
    connect(timerFisica, &QTimer::timeout, this, &balon::actualizarFisica);
    timerFisica->start(16);
}

void balon::patear(float fuerzaX, float fuerzaY) {
    velX = fuerzaX;
    velY = fuerzaY;
}

void balon::reiniciarPosicion(float x, float y) {
    posX = x;
    posY = y;
    velX = 0;
    velY = 0;
    setPos(posX, posY);
}

void balon::actualizarFisica() {
    if (std::abs(velX) < 0.05f && std::abs(velY) < 0.05f) {
        velX = 0;
        velY = 0;
        return;
    }

    posX += velX;
    posY += velY;

    // Aplicar fricción del césped
    velX *= friccion;
    velY *= friccion;

    // Rebotes en los límites internos de la cancha (800x600)
    if (posX <= 35) {
        posX = 35;
        velX = -velX * 0.8f;
    } else if (posX >= 745) {
        posX = 745;
        velX = -velX * 0.8f;
    }

    if (posY <= 25) {
        posY = 25;
        velY = -velY * 0.8f;
    } else if (posY >= 555) {
        posY = 555;
        velY = -velY * 0.8f;
    }

    setPos(posX, posY);
}