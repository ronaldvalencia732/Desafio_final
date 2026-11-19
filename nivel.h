#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QString>

class nivel : public QObject {
    Q_OBJECT

private:
    int numeroNivel;
    QString nombreFase;
    QString rivalNombre;
    int golesObjetivo;
    int tiempoLimite;
    int cantidadDefensores;
    float multiplicadorVelocidadRival;
    QString rutaFondo;

public:
    explicit nivel(QObject *parent = nullptr);

    void cargarNivel(int num);

    int getNumeroNivel() const { return numeroNivel; }
    QString getNombreFase() const { return nombreFase; }
    QString getRivalNombre() const { return rivalNombre; }
    int getGolesObjetivo() const { return golesObjetivo; }
    int getTiempoLimite() const { return tiempoLimite; }
    int getCantidadDefensores() const { return cantidadDefensores; }
    float getMultiplicadorVelocidad() const { return multiplicadorVelocidadRival; }
    QString getRutaFondo() const { return rutaFondo; }
};

#endif // NIVEL_H