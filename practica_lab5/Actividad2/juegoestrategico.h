#ifndef JUEGOESTRATEGICO_H
#define JUEGOESTRATEGICO_H

#include "jugador.h"
#include "proyectil.h"
#include "vector2d.h"

#include <QDoubleSpinBox>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

class Infraestructura;

class JuegoEstrategico : public QWidget
{
    Q_OBJECT

public:
    explicit JuegoEstrategico(QWidget *parent = nullptr);
    ~JuegoEstrategico() override;

private slots:
    void lanzarProyectil();
    void actualizarFisica();
    void reiniciarJuego();
    void actualizarVistaCanon();

private:
    struct ResultadoChoqueRectangulo
    {
        bool hayChoque = false;
        Vector2D normal;
        double penetracion = 0.0;
    };

    void construirInterfaz();
    void crearEscenario();
    void crearInfraestructuraJugador(Jugador &jugador);
    void dibujarFondo();
    void finalizarTurno(const QString &razon);
    void cambiarTurno();
    void actualizarPanel();
    void agregarEvento(const QString &texto);
    void revisarColisionesInfraestructura();
    void revisarColisionRepresentante();
    bool hayGanador() const;
    Jugador &jugadorActual();
    Jugador &rivalActual();
    ResultadoChoqueRectangulo calcularChoqueCirculoRectangulo(const Proyectil &proyectil, const QRectF &rectangulo) const;

    QGraphicsView *m_vista;
    QGraphicsScene *m_escena;
    QLabel *m_lblTurno;
    QLabel *m_lblEstado;
    QLabel *m_lblDatos;
    QDoubleSpinBox *m_spinAngulo;
    QDoubleSpinBox *m_spinVelocidad;
    QPushButton *m_btnLanzar;
    QPushButton *m_btnReiniciar;
    QTextEdit *m_log;
    QTimer m_timer;

    QRectF m_caja;
    Jugador m_jugador1;
    Jugador m_jugador2;
    int m_turno;
    Proyectil *m_proyectil;
    double m_tiempoProyectil;
    int m_numeroTurno;

    const double m_deltaTiempo;
    const double m_gravedad;
    const double m_masaProyectil;
    const double m_radioProyectil;
    const double m_factorDanoInfraestructura;
    const double m_factorDanoRepresentante;
};

#endif // JUEGOESTRATEGICO_H
