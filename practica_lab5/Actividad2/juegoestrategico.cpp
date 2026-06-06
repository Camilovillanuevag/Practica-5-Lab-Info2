#include "juegoestrategico.h"

#include "infraestructura.h"

#include <QBrush>
#include <QColor>
#include <QDateTime>
#include <QFont>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QVBoxLayout>
#include <QtMath>
#include <algorithm>
#include <cmath>

JuegoEstrategico::JuegoEstrategico(QWidget *parent)
    : QWidget(parent),
    m_vista(nullptr),
    m_escena(nullptr),
    m_lblTurno(nullptr),
    m_lblEstado(nullptr),
    m_lblDatos(nullptr),
    m_spinAngulo(nullptr),
    m_spinVelocidad(nullptr),
    m_btnLanzar(nullptr),
    m_btnReiniciar(nullptr),
    m_log(nullptr),
    m_caja(20.0, 20.0, 960.0, 520.0),
    m_jugador1(1, "Jugador 1", true),
    m_jugador2(2, "Jugador 2", false),
    m_turno(1),
    m_proyectil(nullptr),
    m_tiempoProyectil(0.0),
    m_numeroTurno(1),
    m_deltaTiempo(0.016),
    m_gravedad(180.0),
    m_masaProyectil(4.0),
    m_radioProyectil(9.0),
    m_factorDanoInfraestructura(0.075),
    m_factorDanoRepresentante(0.045)
{
    construirInterfaz();
    crearEscenario();

    connect(&m_timer, &QTimer::timeout, this, &JuegoEstrategico::actualizarFisica);
    m_timer.start(16);

    setWindowTitle("Practica 5 - Actividad 2: juego por turnos");
    resize(1220, 720);
}

JuegoEstrategico::~JuegoEstrategico()
{
    // La escena elimina sus items automaticamente.
}

void JuegoEstrategico::construirInterfaz()
{
    m_escena = new QGraphicsScene(this);
    m_escena->setSceneRect(0.0, 0.0, 1000.0, 560.0);

    m_vista = new QGraphicsView(m_escena, this);
    m_vista->setRenderHint(QPainter::Antialiasing, true);
    m_vista->setMinimumSize(1000, 610);

    m_lblTurno = new QLabel(this);
    m_lblEstado = new QLabel(this);
    m_lblDatos = new QLabel(this);

    QFont titulo;
    titulo.setBold(true);
    titulo.setPointSize(13);
    m_lblTurno->setFont(titulo);

    m_spinAngulo = new QDoubleSpinBox(this);
    m_spinAngulo->setRange(8.0, 82.0);
    m_spinAngulo->setValue(42.0);
    m_spinAngulo->setSuffix(" grados");
    m_spinAngulo->setSingleStep(1.0);

    m_spinVelocidad = new QDoubleSpinBox(this);
    m_spinVelocidad->setRange(80.0, 360.0);
    m_spinVelocidad->setValue(245.0);
    m_spinVelocidad->setSuffix(" px/s");
    m_spinVelocidad->setSingleStep(5.0);

    m_btnLanzar = new QPushButton("Lanzar proyectil", this);
    m_btnReiniciar = new QPushButton("Reiniciar", this);
    m_log = new QTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setMinimumWidth(260);

    connect(m_btnLanzar, &QPushButton::clicked, this, &JuegoEstrategico::lanzarProyectil);
    connect(m_btnReiniciar, &QPushButton::clicked, this, &JuegoEstrategico::reiniciarJuego);
    connect(m_spinAngulo, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &JuegoEstrategico::actualizarVistaCanon);

    QVBoxLayout *panel = new QVBoxLayout;
    panel->addWidget(m_lblTurno);
    panel->addWidget(m_lblEstado);
    panel->addSpacing(12);
    panel->addWidget(new QLabel("Angulo de lanzamiento:", this));
    panel->addWidget(m_spinAngulo);
    panel->addWidget(new QLabel("Velocidad inicial:", this));
    panel->addWidget(m_spinVelocidad);
    panel->addWidget(m_btnLanzar);
    panel->addWidget(m_btnReiniciar);
    panel->addSpacing(12);
    panel->addWidget(m_lblDatos);
    panel->addWidget(new QLabel("Registro de eventos:", this));
    panel->addWidget(m_log, 1);

    QHBoxLayout *principal = new QHBoxLayout(this);
    principal->addWidget(m_vista, 1);
    principal->addLayout(panel);
    setLayout(principal);
}

void JuegoEstrategico::crearEscenario()
{
    m_escena->clear();
    m_proyectil = nullptr;
    m_tiempoProyectil = 0.0;
    m_turno = 1;
    m_numeroTurno = 1;
    m_jugador1 = Jugador(1, "Jugador 1", true);
    m_jugador2 = Jugador(2, "Jugador 2", false);

    dibujarFondo();

    crearInfraestructuraJugador(m_jugador1);
    crearInfraestructuraJugador(m_jugador2);
    m_jugador1.configurarGraficos(m_escena, m_caja);
    m_jugador2.configurarGraficos(m_escena, m_caja);
    m_jugador1.actualizarCanon(m_spinAngulo->value());
    m_jugador2.actualizarCanon(m_spinAngulo->value());

    m_log->clear();
    agregarEvento("Juego reiniciado. Las paredes son elasticas y los obstaculos son inelasticos.");
    actualizarPanel();
}

void JuegoEstrategico::crearInfraestructuraJugador(Jugador &jugador)
{
    const bool izquierda = jugador.ladoIzquierdo();
    const double xBase = izquierda ? m_caja.left() + 75.0 : m_caja.right() - 285.0;
    const double yBase = m_caja.bottom() - 180.0;

    Infraestructura *muroIzquierdo = new Infraestructura(jugador.id(),
                                                         "Muro izquierdo",
                                                         QRectF(xBase, yBase + 40.0, 55.0, 140.0),
                                                         200.0,
                                                         0.55);
    Infraestructura *techo = new Infraestructura(jugador.id(),
                                                 "Techo",
                                                 QRectF(xBase, yBase, 210.0, 42.0),
                                                 100.0,
                                                 0.50);
    Infraestructura *muroDerecho = new Infraestructura(jugador.id(),
                                                       "Muro derecho",
                                                       QRectF(xBase + 155.0, yBase + 40.0, 55.0, 140.0),
                                                       200.0,
                                                       0.55);

    m_escena->addItem(muroIzquierdo);
    m_escena->addItem(techo);
    m_escena->addItem(muroDerecho);

    jugador.agregarInfraestructura(muroIzquierdo);
    jugador.agregarInfraestructura(techo);
    jugador.agregarInfraestructura(muroDerecho);
}

void JuegoEstrategico::dibujarFondo()
{
    m_escena->setBackgroundBrush(QBrush(QColor("#f7fafc")));
    m_escena->addRect(m_caja, QPen(QColor("#111827"), 2.5), QBrush(Qt::NoBrush));

    // Linea de suelo y guia central.
    m_escena->addLine(m_caja.left(), m_caja.bottom(), m_caja.right(), m_caja.bottom(), QPen(QColor("#374151"), 2.0));
    m_escena->addLine(m_caja.center().x(), m_caja.top(), m_caja.center().x(), m_caja.bottom(), QPen(QColor("#cbd5e1"), 1.0, Qt::DashLine));

    QGraphicsTextItem *titulo = m_escena->addText("Juego de estrategia por turnos - Practica 5");
    QFont fuente;
    fuente.setBold(true);
    fuente.setPointSize(13);
    titulo->setFont(fuente);
    titulo->setDefaultTextColor(QColor("#111827"));
    titulo->setPos(m_caja.center().x() - titulo->boundingRect().width() / 2.0, 0.0);
}

void JuegoEstrategico::lanzarProyectil()
{
    if (m_proyectil || hayGanador()) {
        return;
    }

    Jugador &jugador = jugadorActual();
    const double angulo = m_spinAngulo->value();
    const double rapidez = m_spinVelocidad->value();
    const double rad = qDegreesToRadians(angulo);
    const double signoX = jugador.ladoIzquierdo() ? 1.0 : -1.0;

    const Vector2D posicionInicial(jugador.puntoLanzamiento().x(), jugador.puntoLanzamiento().y());
    const Vector2D velocidadInicial(signoX * rapidez * std::cos(rad),
                                    -rapidez * std::sin(rad));

    m_proyectil = new Proyectil(posicionInicial, velocidadInicial, m_masaProyectil, m_radioProyectil);
    m_escena->addItem(m_proyectil);
    m_tiempoProyectil = 0.0;
    m_btnLanzar->setEnabled(false);
    m_spinAngulo->setEnabled(false);
    m_spinVelocidad->setEnabled(false);

    agregarEvento(QString("Turno %1: %2 lanza con angulo %3 grados y velocidad %4 px/s.")
                      .arg(m_numeroTurno)
                      .arg(jugador.nombre())
                      .arg(angulo, 0, 'f', 1)
                      .arg(rapidez, 0, 'f', 1));
    actualizarPanel();
}

void JuegoEstrategico::actualizarFisica()
{
    if (!m_proyectil || !m_proyectil->estaActivo() || hayGanador()) {
        return;
    }

    m_tiempoProyectil += m_deltaTiempo;
    m_proyectil->avanzar(m_deltaTiempo, m_gravedad);
    m_proyectil->resolverChoqueConParedes(m_caja);
    revisarColisionesInfraestructura();
    revisarColisionRepresentante();

    if (!m_proyectil) {
        return;
    }

    if (m_tiempoProyectil > 12.0) {
        finalizarTurno("el proyectil supero el tiempo maximo de vuelo");
        return;
    }

    if (m_tiempoProyectil > 2.0 && m_proyectil->rapidez() < 35.0) {
        finalizarTurno("el proyectil perdio casi toda su energia");
        return;
    }

    actualizarPanel();
}

void JuegoEstrategico::revisarColisionesInfraestructura()
{
    if (!m_proyectil) {
        return;
    }

    // Se revisan ambos jugadores porque el proyectil tambien puede golpear infraestructura propia.
    QVector<Infraestructura *> todos = m_jugador1.infraestructura();
    todos += m_jugador2.infraestructura();

    for (Infraestructura *infra : todos) {
        if (!infra || infra->destruida()) {
            continue;
        }

        ResultadoChoqueRectangulo choque = calcularChoqueCirculoRectangulo(*m_proyectil, infra->rectanguloEscena());
        if (!choque.hayChoque) {
            continue;
        }

        const double rapidezImpacto = m_proyectil->rapidez();
        const double dano = m_factorDanoInfraestructura * m_proyectil->masa() * rapidezImpacto;
        infra->aplicarDano(dano);
        infra->actualizarTexto();

        m_proyectil->desplazarFueraDeSuperficie(choque.normal, choque.penetracion + 1.0);
        m_proyectil->rebotarConNormal(choque.normal, infra->restitucion());

        agregarEvento(QString("Impacto contra %1 de Jugador %2. Momento = %3. Dano = %4. Resistencia restante = %5.")
                          .arg(infra->nombre())
                          .arg(infra->propietario())
                          .arg(m_proyectil->masa() * rapidezImpacto, 0, 'f', 1)
                          .arg(dano, 0, 'f', 1)
                          .arg(infra->resistencia(), 0, 'f', 1));

        m_jugador1.actualizarTexto();
        m_jugador2.actualizarTexto();
        return;
    }
}

void JuegoEstrategico::revisarColisionRepresentante()
{
    if (!m_proyectil) {
        return;
    }

    Jugador &rival = rivalActual();
    QGraphicsEllipseItem *representante = rival.representanteItem();
    if (!representante || !representante->collidesWithItem(m_proyectil)) {
        return;
    }

    const double rapidezImpacto = m_proyectil->rapidez();
    const double dano = m_factorDanoRepresentante * m_proyectil->masa() * rapidezImpacto;
    rival.aplicarDanoRepresentante(dano);

    agregarEvento(QString("El representante de %1 fue golpeado. Momento = %2. Dano = %3. Vida restante = %4.")
                      .arg(rival.nombre())
                      .arg(m_proyectil->masa() * rapidezImpacto, 0, 'f', 1)
                      .arg(dano, 0, 'f', 1)
                      .arg(rival.vidaRepresentante(), 0, 'f', 1));

    if (rival.representanteDerrotado()) {
        m_btnLanzar->setEnabled(false);
        m_spinAngulo->setEnabled(false);
        m_spinVelocidad->setEnabled(false);
        if (m_proyectil) {
            m_escena->removeItem(m_proyectil);
            delete m_proyectil;
            m_proyectil = nullptr;
        }
        actualizarPanel();
        QMessageBox::information(this, "Fin del juego", jugadorActual().nombre() + " gana la partida.");
        return;
    }

    finalizarTurno("el proyectil golpeo al representante rival");
}

void JuegoEstrategico::finalizarTurno(const QString &razon)
{
    if (m_proyectil) {
        m_escena->removeItem(m_proyectil);
        delete m_proyectil;
        m_proyectil = nullptr;
    }

    agregarEvento("Fin del turno: " + razon + ".");
    cambiarTurno();
    actualizarPanel();
}

void JuegoEstrategico::cambiarTurno()
{
    m_turno = (m_turno == 1) ? 2 : 1;
    ++m_numeroTurno;

    if (!hayGanador()) {
        m_btnLanzar->setEnabled(true);
        m_spinAngulo->setEnabled(true);
        m_spinVelocidad->setEnabled(true);
        actualizarVistaCanon();
    }
}

void JuegoEstrategico::actualizarPanel()
{
    Jugador &actual = jugadorActual();
    Jugador &rival = rivalActual();

    m_lblTurno->setText(QString("Turno de %1").arg(actual.nombre()));

    if (hayGanador()) {
        const QString ganador = m_jugador1.representanteDerrotado() ? m_jugador2.nombre() : m_jugador1.nombre();
        m_lblEstado->setText("Ganador: " + ganador);
    } else if (m_proyectil) {
        m_lblEstado->setText(QString("Proyectil en vuelo - t = %1 s, rapidez = %2 px/s")
                                 .arg(m_tiempoProyectil, 0, 'f', 2)
                                 .arg(m_proyectil->rapidez(), 0, 'f', 1));
    } else {
        m_lblEstado->setText("Ajuste angulo y velocidad para lanzar.");
    }

    m_lblDatos->setText(QString("Vida J1: %1 | Vida J2: %2\nInfraestructura rival: %3\nDano infraestructura = %4 x masa x rapidez\nDano representante = %5 x masa x rapidez")
                            .arg(m_jugador1.vidaRepresentante(), 0, 'f', 1)
                            .arg(m_jugador2.vidaRepresentante(), 0, 'f', 1)
                            .arg(rival.infraestructuraDestruida() ? "destruida" : "activa")
                            .arg(m_factorDanoInfraestructura, 0, 'f', 3)
                            .arg(m_factorDanoRepresentante, 0, 'f', 3));

    m_jugador1.actualizarTexto();
    m_jugador2.actualizarTexto();
}

void JuegoEstrategico::agregarEvento(const QString &texto)
{
    m_log->append("- " + texto);
}

void JuegoEstrategico::reiniciarJuego()
{
    crearEscenario();
    m_btnLanzar->setEnabled(true);
    m_spinAngulo->setEnabled(true);
    m_spinVelocidad->setEnabled(true);
}

void JuegoEstrategico::actualizarVistaCanon()
{
    m_jugador1.actualizarCanon(m_spinAngulo->value());
    m_jugador2.actualizarCanon(m_spinAngulo->value());
}

bool JuegoEstrategico::hayGanador() const
{
    return m_jugador1.representanteDerrotado() || m_jugador2.representanteDerrotado();
}

Jugador &JuegoEstrategico::jugadorActual()
{
    return (m_turno == 1) ? m_jugador1 : m_jugador2;
}

Jugador &JuegoEstrategico::rivalActual()
{
    return (m_turno == 1) ? m_jugador2 : m_jugador1;
}

JuegoEstrategico::ResultadoChoqueRectangulo JuegoEstrategico::calcularChoqueCirculoRectangulo(const Proyectil &proyectil, const QRectF &rectangulo) const
{
    ResultadoChoqueRectangulo resultado;
    const Vector2D centro = proyectil.posicionFisica();
    const double radio = proyectil.radio();

    const double puntoCercanoX = std::clamp(centro.x, rectangulo.left(), rectangulo.right());
    const double puntoCercanoY = std::clamp(centro.y, rectangulo.top(), rectangulo.bottom());

    Vector2D normal(centro.x - puntoCercanoX, centro.y - puntoCercanoY);
    double distancia = normal.longitud();

    if (distancia > radio) {
        return resultado;
    }

    if (qFuzzyIsNull(distancia)) {
        // Si el centro quedo dentro del rectangulo, se escoge el lado mas cercano.
        const double izquierda = std::abs(centro.x - rectangulo.left());
        const double derecha = std::abs(rectangulo.right() - centro.x);
        const double arriba = std::abs(centro.y - rectangulo.top());
        const double abajo = std::abs(rectangulo.bottom() - centro.y);
        const double minimo = std::min(std::min(izquierda, derecha), std::min(arriba, abajo));

        if (minimo == izquierda) {
            normal = Vector2D(-1.0, 0.0);
            distancia = izquierda;
        } else if (minimo == derecha) {
            normal = Vector2D(1.0, 0.0);
            distancia = derecha;
        } else if (minimo == arriba) {
            normal = Vector2D(0.0, -1.0);
            distancia = arriba;
        } else {
            normal = Vector2D(0.0, 1.0);
            distancia = abajo;
        }
    }

    resultado.hayChoque = true;
    resultado.normal = normal.normalizado();
    resultado.penetracion = std::max(0.0, radio - distancia);
    return resultado;
}

