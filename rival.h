#ifndef RIVAL_H
#define RIVAL_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QString>

class rival : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    QString rol; // "Arquero" o "Defensor"
    float posX;
    float posY;
    float velocidadY;
    float limiteSuperior;
    float limiteInferior;

    QPixmap spriteSheet;
    int currentFrame;
    QTimer *timerIA;

    void recortarSprite(int row, int col);

public:
    rival(QString tipoRol, float x, float y, float limSup, float limInf, QGraphicsItem *parent = nullptr);

    void setMultiplicadorVelocidad(float factor) { velocidadY *= factor; }

public slots:
    void moverAutomatico();
};

#endif // RIVAL_H