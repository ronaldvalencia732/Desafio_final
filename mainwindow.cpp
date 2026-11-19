#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFont>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ultimaDirX = 1.0f;
    ultimaDirY = 0.0f;
    juegoPausado = false;

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, 800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocus();
    setFixedSize(800, 600);

    // Instanciar gestor de niveles, balón y jugador
    gestorNivel = new nivel(this);
    pelota = new balon();
    scene->addItem(pelota);

    jugadorPrincipal = new jugador("Macnelly");
    scene->addItem(jugadorPrincipal);

    // Configurar textos del HUD
    textoFase = new QGraphicsTextItem();
    textoFase->setFont(QFont("Arial", 11, QFont::Bold));
    textoFase->setDefaultTextColor(Qt::yellow);
    textoFase->setPos(20, 6);
    scene->addItem(textoFase);

    textoMarcador = new QGraphicsTextItem();
    textoMarcador->setFont(QFont("Arial", 13, QFont::Bold));
    textoMarcador->setDefaultTextColor(Qt::white);
    textoMarcador->setPos(320, 5);
    scene->addItem(textoMarcador);

    textoTiempo = new QGraphicsTextItem();
    textoTiempo->setFont(QFont("Arial", 13, QFont::Bold));
    textoTiempo->setDefaultTextColor(Qt::cyan);
    textoTiempo->setPos(650, 5);
    scene->addItem(textoTiempo);

    textoMensajeCentral = new QGraphicsTextItem();
    textoMensajeCentral->setFont(QFont("Arial", 16, QFont::Bold));
    textoMensajeCentral->setZValue(10); // Siempre encima de todo
    scene->addItem(textoMensajeCentral);

    // Timers de física/goles y reloj de partido
    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::cicloDeJuego);
    timerJuego->start(20);

    timerReloj = new QTimer(this);
    connect(timerReloj, &QTimer::timeout, this, &MainWindow::actualizarReloj);
    timerReloj->start(1000);

    // Arrancar en el Nivel 1
    iniciarNivel(1);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mostrarMensajeCentrado(const QString &mensaje, const QColor &color) {
    textoMensajeCentral->setDefaultTextColor(color);
    textoMensajeCentral->setPlainText(mensaje);

    // Calcular el centro exacto en pantalla (800x600) según el ancho real del texto
    qreal anchoTexto = textoMensajeCentral->boundingRect().width();
    qreal altoTexto = textoMensajeCentral->boundingRect().height();
    textoMensajeCentral->setPos((800.0 - anchoTexto) / 2.0, (600.0 - altoTexto) / 2.0);
}

void MainWindow::limpiarRivales() {
    for (rival *r : listaRivales) {
        scene->removeItem(r);
        delete r;
    }
    listaRivales.clear();
}

void MainWindow::iniciarNivel(int numero) {
    gestorNivel->cargarNivel(numero);

    golesNacional = 0;
    golesRival = 0;
    segundosRestantes = gestorNivel->getTiempoLimite();
    juegoPausado = false;
    textoMensajeCentral->setPlainText("");

    // Cargar fondo del nivel
    QPixmap fondo(gestorNivel->getRutaFondo());
    if (!fondo.isNull()) {
        scene->setBackgroundBrush(fondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    // Reiniciar posiciones de balón y jugador
    pelota->reiniciarPosicion(400, 300);
    jugadorPrincipal->setPos(250, 280);

    // Crear rivales según dificultad del nivel
    limpiarRivales();
    float mult = gestorNivel->getMultiplicadorVelocidad();

    rival *arquero = new rival("Arquero", 715, 275, 220, 330);
    arquero->setMultiplicadorVelocidad(mult);
    listaRivales << arquero;

    int defs = gestorNivel->getCantidadDefensores();
    if (defs >= 1) {
        rival *d1 = new rival("Defensor", 520, 180, 90, 320);
        d1->setMultiplicadorVelocidad(mult);
        listaRivales << d1;
    }
    if (defs >= 2) {
        rival *d2 = new rival("Defensor", 600, 380, 250, 500);
        d2->setMultiplicadorVelocidad(mult);
        listaRivales << d2;
    }
    if (defs >= 3) {
        rival *d3 = new rival("Defensor", 450, 280, 120, 460);
        d3->setMultiplicadorVelocidad(mult);
        listaRivales << d3;
    }

    for (rival *r : listaRivales) {
        scene->addItem(r);
    }

    actualizarHUD();
}

void MainWindow::actualizarHUD() {
    textoFase->setPlainText(gestorNivel->getNombreFase());
    textoMarcador->setPlainText(
        QString("NAL %1/%2 - %3 %4")
            .arg(golesNacional)
            .arg(gestorNivel->getGolesObjetivo())
            .arg(golesRival)
            .arg(gestorNivel->getRivalNombre())
        );
    textoTiempo->setPlainText(QString("TIEMPO: %1s").arg(segundosRestantes));
}

void MainWindow::actualizarReloj() {
    if (juegoPausado) return;

    segundosRestantes--;
    actualizarHUD();

    if (segundosRestantes <= 0) {
        juegoPausado = true;
        mostrarMensajeCentrado("TIEMPO AGOTADO - PRESIONA 'R' PARA REINTENTAR", Qt::white);
    }
}

void MainWindow::cicloDeJuego() {
    if (juegoPausado) return;

    float bx = pelota->getPosX();
    float by = pelota->getPosY();

    // 1. Verificar GOL a favor en arco derecho
    if (bx >= 740 && by >= 235 && by <= 365) {
        golesNacional++;
        actualizarHUD();

        // Verificar si alcanzó el objetivo de goles del nivel
        if (golesNacional >= gestorNivel->getGolesObjetivo()) {
            if (gestorNivel->getNumeroNivel() < 3) {
                iniciarNivel(gestorNivel->getNumeroNivel() + 1);
            } else {
                juegoPausado = true;
                mostrarMensajeCentrado("¡CAMPEÓN DE LA LIBERTADORES! - PRESIONA 'R' PARA REINTENTAR", QColor(50, 255, 100));
            }
            return;
        }

        pelota->reiniciarPosicion(400, 300);
        return;
    }

    // 2. Verificar GOL en contra en arco izquierdo
    if (bx <= 42 && by >= 235 && by <= 365) {
        golesRival++;
        actualizarHUD();
        pelota->reiniciarPosicion(400, 300);
        return;
    }

    // 3. Intercepción / despeje de los rivales
    for (rival *r : listaRivales) {
        if (pelota->collidesWithItem(r)) {
            float despejeY = (by < r->y() + 20) ? -4.5f : 4.5f;
            pelota->patear(-9.5f, despejeY);
            break;
        }
    }
}

void MainWindow::verificarContactoBalon() {
    if (jugadorPrincipal->collidesWithItem(pelota)) {
        pelota->patear(ultimaDirX * 4.5f, ultimaDirY * 4.5f);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Permitir reiniciar nivel o torneo con tecla R
    if (event->key() == Qt::Key_R) {
        if (gestorNivel->getNumeroNivel() == 3 && golesNacional >= gestorNivel->getGolesObjetivo()) {
            iniciarNivel(1);
        } else {
            iniciarNivel(gestorNivel->getNumeroNivel());
        }
        return;
    }

    if (juegoPausado || jugadorPrincipal == nullptr || pelota == nullptr) return;

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
        float dx = pelota->x() - (jugadorPrincipal->x() + 20);
        float dy = pelota->y() - (jugadorPrincipal->y() + 25);
        float distancia = std::sqrt(dx * dx + dy * dy);

        if (distancia < 55.0f) {
            pelota->patear(ultimaDirX * 15.0f, ultimaDirY * 15.0f);
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