#include "proyectil.h"

#include <QBrush>
#include <QColor>
#include <QtGlobal>
#include <algorithm>

Proyectil::Proyectil(const Vector2D &posicionInicial,
                     const Vector2D &velocidadInicial,
                     double masa,
                     double radio,
                     QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
      m_posicion(posicionInicial),
      m_velocidad(velocidadInicial),
      m_masa(masa),
      m_radio(radio),
      m_activo(true)
{
    setRect(-m_radio, -m_radio, 2.0 * m_radio, 2.0 * m_radio);
    setBrush(QBrush(QColor("#d00000")));
    setPen(QPen(QColor("#7f0000"), 1.5));
    setZValue(10.0);
    setPos(m_posicion.x, m_posicion.y);
}

Vector2D Proyectil::posicionFisica() const
{
    return m_posicion;
}

Vector2D Proyectil::velocidad() const
{
    return m_velocidad;
}

double Proyectil::masa() const
{
    return m_masa;
}

double Proyectil::radio() const
{
    return m_radio;
}

double Proyectil::rapidez() const
{
    return m_velocidad.longitud();
}

bool Proyectil::estaActivo() const
{
    return m_activo;
}

void Proyectil::setVelocidad(const Vector2D &nuevaVelocidad)
{
    m_velocidad = nuevaVelocidad;
}

void Proyectil::setPosicionFisica(const Vector2D &nuevaPosicion)
{
    m_posicion = nuevaPosicion;
    setPos(m_posicion.x, m_posicion.y);
}

void Proyectil::setActivo(bool valor)
{
    m_activo = valor;
}

void Proyectil::avanzar(double deltaTiempo, double gravedad)
{
    if (!m_activo) {
        return;
    }

    // En la pantalla y crece hacia abajo, por eso la gravedad es positiva.
    m_velocidad.y += gravedad * deltaTiempo;
    m_posicion += m_velocidad * deltaTiempo;
    setPos(m_posicion.x, m_posicion.y);
}

void Proyectil::resolverChoqueConParedes(const QRectF &caja)
{
    if (!m_activo) {
        return;
    }

    if (m_posicion.x - m_radio < caja.left()) {
        m_posicion.x = caja.left() + m_radio;
        m_velocidad.x = std::abs(m_velocidad.x);
    }
    if (m_posicion.x + m_radio > caja.right()) {
        m_posicion.x = caja.right() - m_radio;
        m_velocidad.x = -std::abs(m_velocidad.x);
    }
    if (m_posicion.y - m_radio < caja.top()) {
        m_posicion.y = caja.top() + m_radio;
        m_velocidad.y = std::abs(m_velocidad.y);
    }
    if (m_posicion.y + m_radio > caja.bottom()) {
        m_posicion.y = caja.bottom() - m_radio;
        m_velocidad.y = -std::abs(m_velocidad.y);
    }

    setPos(m_posicion.x, m_posicion.y);
}

void Proyectil::rebotarConNormal(const Vector2D &normalEntrada, double restitucion)
{
    const Vector2D normal = normalEntrada.normalizado();
    if (qFuzzyIsNull(normal.longitudCuadrada())) {
        return;
    }

    const Vector2D componentePerpendicular = normal * m_velocidad.productoPunto(normal);
    const Vector2D componenteParalela = m_velocidad - componentePerpendicular;

    // Rebote inelastico: se invierte y reduce solo la componente perpendicular.
    m_velocidad = componenteParalela - componentePerpendicular * restitucion;
}

void Proyectil::desplazarFueraDeSuperficie(const Vector2D &normal, double distancia)
{
    const Vector2D n = normal.normalizado();
    if (qFuzzyIsNull(n.longitudCuadrada())) {
        return;
    }
    m_posicion += n * distancia;
    setPos(m_posicion.x, m_posicion.y);
}
