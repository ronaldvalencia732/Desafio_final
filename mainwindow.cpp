#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 1. Crear la escena de juego (Resolución 800x600)
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    // 2. Cargar el fondo de la cancha desde el archivo de recursos
    QPixmap fondo(":/spritesJuego/cancha.jpeg");
    if (!fondo.isNull()) {
        scene->setBackgroundBrush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    // 3. Crear el visor gráfico y asignarle la escena
    view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, 800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Ajustar el tamaño de la ventana principal
    setFixedSize(800, 600);

    // 4. Instanciar y agregar al jugador verdolaga
    jugadorPrincipal = new jugador("Macnelly");
    scene->addItem(jugadorPrincipal);
}

MainWindow::~MainWindow() {
    delete ui;
}

// 5. Manejo de movimiento del jugador por teclado
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (jugadorPrincipal == nullptr) return;

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        jugadorPrincipal->mover(0, -1);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        jugadorPrincipal->mover(0, 1);
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        jugadorPrincipal->mover(-1, 0);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        jugadorPrincipal->mover(1, 0);
        break;
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