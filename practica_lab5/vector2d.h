#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QtGlobal>
#include <cmath>

class Vector2D
{
public:
    double x;
    double y;

    Vector2D(double valorX = 0.0, double valorY = 0.0)
        : x(valorX), y(valorY)
    {
    }

    double longitud() const
    {
        return std::sqrt(x * x + y * y);
    }

    double longitudCuadrada() const
    {
        return x * x + y * y;
    }

    Vector2D normalizado() const
    {
        const double len = longitud();
        if (qFuzzyIsNull(len)) {
            return Vector2D();
        }
        return Vector2D(x / len, y / len);
    }

    double productoPunto(const Vector2D &otro) const
    {
        return x * otro.x + y * otro.y;
    }

    Vector2D operator+(const Vector2D &otro) const
    {
        return Vector2D(x + otro.x, y + otro.y);
    }

    Vector2D operator-(const Vector2D &otro) const
    {
        return Vector2D(x - otro.x, y - otro.y);
    }

    Vector2D operator*(double escalar) const
    {
        return Vector2D(x * escalar, y * escalar);
    }

    Vector2D operator/(double escalar) const
    {
        return Vector2D(x / escalar, y / escalar);
    }

    Vector2D &operator+=(const Vector2D &otro)
    {
        x += otro.x;
        y += otro.y;
        return *this;
    }
};

#endif
