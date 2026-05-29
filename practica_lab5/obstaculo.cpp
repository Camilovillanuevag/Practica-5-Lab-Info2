#include "obstaculo.h"

Obstaculo::Obstaculo() {}

double obtenerIzquierda(){
    return posicionX;
}
double obtenerDerecha() {
    return posicionX + ancho;
}
double obtenerArriba() {
    return posicionY;
}
double obtenerAbajo() {
    return posicionY + alto;
}

double obtenerRebote() {
    return rebote;
}