#include "colisiones.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <cmath>

Colisiones::Colisiones()
    : m_limites(0.0, 0.0, 800.0, 500.0),
    m_deltaTiempo(0.05),
    m_tiempoTotal(35.0),
    m_tiempoActual(0.0)
{
}

void Colisiones::configurarEscenarioPredeterminado()
{
    m_particulas.clear();
    m_obstaculos.clear();
    m_trayectorias.clear();
    m_eventosColision.clear();
    m_tiempoActual = 0.0;

    m_particulas.append(Particula(1, Vector2D(75.0, 80.0), Vector2D(68.0, 41.0), 2.0, 12.0, QColor("#e63946")));
    m_particulas.append(Particula(2, Vector2D(700.0, 95.0), Vector2D(-62.0, 36.0), 1.5, 10.0, QColor("#2a9d8f")));
    m_particulas.append(Particula(3, Vector2D(115.0, 410.0), Vector2D(54.0, -48.0), 2.8, 14.0, QColor("#457b9d")));
    m_particulas.append(Particula(4, Vector2D(690.0, 390.0), Vector2D(-58.0, -42.0), 1.9, 11.0, QColor("#f4a261")));
    m_particulas.append(Particula(5, Vector2D(410.0, 255.0), Vector2D(31.0, -55.0), 1.2, 9.0, QColor("#7b2cbf")));

    m_obstaculos.append(Obstaculo(1, QRectF(220.0, 120.0, 60.0, 60.0), 0.55));
    m_obstaculos.append(Obstaculo(2, QRectF(515.0, 110.0, 70.0, 70.0), 0.60));
    m_obstaculos.append(Obstaculo(3, QRectF(180.0, 315.0, 75.0, 75.0), 0.50));
    m_obstaculos.append(Obstaculo(4, QRectF(540.0, 305.0, 65.0, 65.0), 0.58));

    guardarPosicionesActuales();
}

void Colisiones::ejecutar()
{
    while (m_tiempoActual < m_tiempoTotal) {
        avanzarPaso();
    }
}

bool Colisiones::exportarArchivosTexto(const QString &rutaDirectorio) const
{
    QDir dir(rutaDirectorio);
    if (!dir.exists() && !dir.mkpath(".")) {
        return false;
    }

    QFile archivoTrayectoria(dir.filePath("trayectorias.csv"));
    if (!archivoTrayectoria.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream salidaTrayectoria(&archivoTrayectoria);
    salidaTrayectoria << "tiempo,id_particula,x,y,vx,vy,masa,radio\n";
    for (const Particula &particula : m_particulas) {
        const QVector<PuntoTrayectoria> puntos = m_trayectorias.value(particula.id());
        for (const PuntoTrayectoria &punto : puntos) {
            salidaTrayectoria << QString::number(punto.tiempo, 'f', 3) << ","
                              << particula.id() << ","
                              << QString::number(punto.posicion.x, 'f', 3) << ","
                              << QString::number(punto.posicion.y, 'f', 3) << ","
                              << QString::number(punto.velocidad.x, 'f', 3) << ","
                              << QString::number(punto.velocidad.y, 'f', 3) << ","
                              << QString::number(punto.masa, 'f', 3) << ","
                              << QString::number(punto.radio, 'f', 3) << "\n";
        }
    }

    QFile archivoColisiones(dir.filePath("colisiones.txt"));
    if (!archivoColisiones.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream salidaColisiones(&archivoColisiones);
    salidaColisiones << "Registro de colisiones\n";
    salidaColisiones << "tiempo | evento\n";
    for (const EventoColision &evento : m_eventosColision) {
        salidaColisiones << QString::number(evento.tiempo, 'f', 3) << " | " << evento.descripcion << "\n";
    }

    return true;
}

