#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QRectF>

class Obstaculo
{
public:
    Obstaculo(int idObstaculo = 0, const QRectF &rectangulo = QRectF(), double restitucion = 0.65);

    int id() const;
    QRectF rectangulo() const;
    double restitucion() const;

private:
    int m_id;
    QRectF m_rectangulo;
    double m_restitution;
};

#endif
