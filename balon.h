#ifndef BALON_H
#define BALON_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class balon : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    float posX;
    float posY;
    float velX;
    float velY;
    float friccion;
    QTimer *timerFisica;

public:
    balon(QGraphicsItem *parent = nullptr);

    void patear(float fuerzaX, float fuerzaY);
    void reiniciarPosicion(float x = 400, float y = 300);

    float getPosX() const { return posX; }
    float getPosY() const { return posY; }

public slots:
    void actualizarFisica();
};

#endif // BALON_H