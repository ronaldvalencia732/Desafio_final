#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <QColor>
#include "jugador.h"
#include "balon.h"
#include "rival.h"
#include "nivel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void cicloDeJuego();
    void actualizarReloj();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;

    nivel *gestorNivel;
    jugador *jugadorPrincipal;
    balon *pelota;
    QList<rival*> listaRivales;

    QGraphicsTextItem *textoFase;
    QGraphicsTextItem *textoMarcador;
    QGraphicsTextItem *textoTiempo;
    QGraphicsTextItem *textoMensajeCentral;

    QTimer *timerJuego;
    QTimer *timerReloj;

    int golesNacional;
    int golesRival;
    int segundosRestantes;
    bool juegoPausado;

    float ultimaDirX;
    float ultimaDirY;

    void iniciarNivel(int numero);
    void limpiarRivales();
    void verificarContactoBalon();
    void actualizarHUD();
    void mostrarMensajeCentrado(const QString &mensaje, const QColor &color);
};

#endif // MAINWINDOW_H