#include "particula.h"

#include <iostream>
#include <cmath>

Particula::Particula(double x, double y, double vx, double vy, double masa, double radio)
    : x(x), y(y), vx(vx), vy(vy), masa(masa), radio(radio), activa(true) {}

    void mover(double dt) {
        x = x + vx * dt;
        y = y + vy * dt;
    }

    double distancia(Particle otra) {
        double dx = x - otra.x;
        double dy = y - otra.y;

        return sqrt(dx * dx + dy * dy);
    }

    double rapidez() {
        return sqrt(vx * vx + vy * vy);
    }

    // Momento lineal
    double momento() {
        return masa * rapidez();
    }
};