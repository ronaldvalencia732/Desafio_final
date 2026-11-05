#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>

class jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    QString nombre;
    float posicionX;
    float posicionY;
    float velocidad;
    QString caracteristicaActiva;
    float resistencia;

    QPixmap spriteSheet;
    int currentFrame;

public:
    jugador(QString nombreJugador = "Macnelly", QGraphicsItem *parent = nullptr);

    void mover(float dx, float dy);
    void recortarSprite(int row, int col);
    void activarHabilidadEspecial(QString habilidad);
};

#endif // JUGADOR_H