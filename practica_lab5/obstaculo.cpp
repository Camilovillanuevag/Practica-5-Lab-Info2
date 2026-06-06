#include "obstaculo.h"

Obstaculo::Obstaculo(int idObstaculo, const QRectF &rectangulo, double restitucion)
    : m_id(idObstaculo), m_rectangulo(rectangulo), m_restitucion(restitucion)
{
}

int Obstaculo::id() const
{
    return m_id;
}

QRectF Obstaculo::rectangulo() const
{
    return m_rectangulo;
}

double Obstaculo::restitucion() const
{
    return m_restitucion;
}
