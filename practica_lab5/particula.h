#ifndef PARTICULA_H
#define PARTICULA_H

#include "vector2d.h"
#include <QColor>

    class Particula
{
public:
    Particula(int idParticula = 0,
              const Vector2D &posicionInicial = Vector2D(),
              const Vector2D &velocidadInicial = Vector2D(),
              double masaParticula = 1.0,
              double radioParticula = 8.0,
              const QColor &colorParticula = Qt::white);

    int id() const;
    Vector2D posicion() const;
    Vector2D velocidad() const;
    double masa() const;
    double radio() const;
    QColor color() const;
    bool estaActiva() const;

    void setPosicion(const Vector2D &nuevaPosicion);
    void setVelocidad(const Vector2D &nuevaVelocidad);
    void setMasa(double nuevaMasa);
    void setRadio(double nuevoRadio);
    void setActiva(bool valor);

    void mover(double deltaTiempo);
    void absorber(const Particula &otra);

private:
    int m_id;
    Vector2D m_posicion;
    Vector2D m_velocidad;
    double m_masa;
    double m_radio;
    QColor m_color;
    bool m_activa;
};

#endif