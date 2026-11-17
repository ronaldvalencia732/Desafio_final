#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ultimaDirX = 1.0f; // Por defecto mira hacia la derecha (arco rival)
    ultimaDirY = 0.0f;

    // 1. Escena de juego
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    // 2. Fondo de la cancha
    QPixmap fondo(":/spritesJuego/cancha.jpeg");
    if (!fondo.isNull()) {
        scene->setBackgroundBrush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    // 3. Visor gráfico
    view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, 800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocus();

    setFixedSize(800, 600);

    // 4. Instanciar Balón en el centro del campo
    pelota = new balon();
    scene->addItem(pelota);

    // 5. Instanciar Jugador Verdolaga
    jugadorPrincipal = new jugador("Macnelly");
    scene->addItem(jugadorPrincipal);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::verificarContactoBalon() {
    // Si el jugador choca o está muy cerca del balón, lo empuja suavemente (conducción)
    if (jugadorPrincipal->collidesWithItem(pelota)) {
        pelota->patear(ultimaDirX * 4.5f, ultimaDirY * 4.5f);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (jugadorPrincipal == nullptr || pelota == nullptr) return;

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        ultimaDirX = 0; ultimaDirY = -1;
        jugadorPrincipal->mover(0, -1);
        verificarContactoBalon();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        ultimaDirX = 0; ultimaDirY = 1;
        jugadorPrincipal->mover(0, 1);
        verificarContactoBalon();
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        ultimaDirX = -1; ultimaDirY = 0;
        jugadorPrincipal->mover(-1, 0);
        verificarContactoBalon();
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        ultimaDirX = 1; ultimaDirY = 0;
        jugadorPrincipal->mover(1, 0);
        verificarContactoBalon();
        break;
    case Qt::Key_Space: {
        // Calcular distancia entre el centro del jugador y el balón para patear fuerte
        float dx = pelota->x() - (jugadorPrincipal->x() + 20);
        float dy = pelota->y() - (jugadorPrincipal->y() + 25);
        float distancia = std::sqrt(dx * dx + dy * dy);

        if (distancia < 55.0f) {
            pelota->patear(ultimaDirX * 14.0f, ultimaDirY * 14.0f);
        }
        break;
    }
    case Qt::Key_1:
        jugadorPrincipal->activarHabilidadEspecial("Veloz");
        break;
    case Qt::Key_2:
        jugadorPrincipal->activarHabilidadEspecial("Arriesgado");
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}