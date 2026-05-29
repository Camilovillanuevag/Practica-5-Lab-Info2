#include "particula.h"

#include <cmath>

Particula::Particula(int idParticula,
                     const Vector2D &posicionInicial,
                     const Vector2D &velocidadInicial,
                     double masaParticula,
                     double radioParticula,
                     const QColor &colorParticula)
    : m_id(idParticula),
    m_posicion(posicionInicial),
    m_velocidad(velocidadInicial),
    m_masa(masaParticula),
    m_radio(radioParticula),
    m_color(colorParticula),
    m_activa(true)
{
}

int Particula::id() const
{
    return m_id;
}

Vector2D Particula::posicion() const
{
    return m_posicion;
}

Vector2D Particula::velocidad() const
{
    return m_velocidad;
}

double Particula::masa() const
{
    return m_masa;
}

double Particula::radio() const
{
    return m_radio;
}

QColor Particula::color() const
{
    return m_color;
}

bool Particula::estaActiva() const
{
    return m_activa;
}

void Particula::setPosicion(const Vector2D &nuevaPosicion)
{
    m_posicion = nuevaPosicion;
}

void Particula::setVelocidad(const Vector2D &nuevaVelocidad)
{
    m_velocidad = nuevaVelocidad;
}

void Particula::setMasa(double nuevaMasa)
{
    m_masa = nuevaMasa;
}

void Particula::setRadio(double nuevoRadio)
{
    m_radio = nuevoRadio;
}

void Particula::setActiva(bool valor)
{
    m_activa = valor;
}

void Particula::mover(double deltaTiempo)
{
    m_posicion += m_velocidad * deltaTiempo;
}

void Particula::absorber(const Particula &otra)
{
    const double masaCombinada = m_masa + otra.m_masa;
    const Vector2D posicionCombinada =
        (m_posicion * m_masa + otra.m_posicion * otra.m_masa) / masaCombinada;
    const Vector2D velocidadCombinada =
        (m_velocidad * m_masa + otra.m_velocidad * otra.m_masa) / masaCombinada;

    m_posicion = posicionCombinada;
    m_velocidad = velocidadCombinada;
    m_masa = masaCombinada;
    m_radio = std::sqrt(m_radio * m_radio + otra.m_radio * otra.m_radio);
}