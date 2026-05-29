#ifndef COLISIONES_H
#define COLISIONES_H

#include "obstaculo.h"
#include "particula.h"

#include <QMap>
#include <QRectF>
#include <QString>
#include <QVector>

struct PuntoTrayectoria
{
    double tiempo;
    Vector2D posicion;
    Vector2D velocidad;
    double masa;
    double radio;
};

struct EventoColision
{
    double tiempo;
    QString descripcion;
};

class Colisiones
{
public:
    Colisiones();

    void configurarEscenarioPredeterminado();
    void ejecutar();
    bool exportarArchivosTexto(const QString &rutaDirectorio) const;

    QRectF limites() const;
    QVector<Particula> particulas() const;
    QVector<Obstaculo> obstaculos() const;
    QMap<int, QVector<PuntoTrayectoria>> trayectorias() const;
    QVector<EventoColision> eventosColision() const;

private:
    void avanzarPaso();
    void guardarPosicionesActuales();
    void resolverColisionesPared(Particula &particula);
    void resolverColisionesObstaculo(Particula &particula);
    void resolverColisionesParticulas();
    bool circuloIntersecaRectangulo(const Particula &particula, const QRectF &rectangulo, Vector2D *normal, double *penetracion) const;
    void registrarColision(const QString &descripcion);

    QRectF m_limites;
    QVector<Particula> m_particulas;
    QVector<Obstaculo> m_obstaculos;
    QMap<int, QVector<PuntoTrayectoria>> m_trayectorias;
    QVector<EventoColision> m_eventosColision;
    double m_deltaTiempo; // o m_pasoTiempo
    double m_tiempoTotal;
    double m_tiempoActual;
};

#endif
