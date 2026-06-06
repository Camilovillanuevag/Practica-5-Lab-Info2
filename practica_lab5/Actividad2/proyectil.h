#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "vector2d.h"

#include <QGraphicsEllipseItem>
#include <QPen>
#include <QRectF>

class Proyectil : public QGraphicsEllipseItem
{
public:
    Proyectil(const Vector2D &posicionInicial,
              const Vector2D &velocidadInicial,
              double masa,
              double radio,
              QGraphicsItem *parent = nullptr);

    Vector2D posicionFisica() const;
    Vector2D velocidad() const;
    double masa() const;
    double radio() const;
    double rapidez() const;
    bool estaActivo() const;

    void setVelocidad(const Vector2D &nuevaVelocidad);
    void setPosicionFisica(const Vector2D &nuevaPosicion);
    void setActivo(bool valor);

    void avanzar(double deltaTiempo, double gravedad);
    void resolverChoqueConParedes(const QRectF &caja);
    void rebotarConNormal(const Vector2D &normal, double restitucion);
    void desplazarFueraDeSuperficie(const Vector2D &normal, double distancia);

private:
    Vector2D m_posicion;
    Vector2D m_velocidad;
    double m_masa;
    double m_radio;
    bool m_activo;
};

#endif
