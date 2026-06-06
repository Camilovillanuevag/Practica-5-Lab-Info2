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

QRectF Colisiones::limites() const
{
    return m_limites;
}

QVector<Particula> Colisiones::particulas() const
{
    return m_particulas;
}

QVector<Obstaculo> Colisiones::obstaculos() const
{
    return m_obstaculos;
}

QMap<int, QVector<PuntoTrayectoria>> Colisiones::trayectorias() const
{
    return m_trayectorias;
}

QVector<EventoColision> Colisiones::eventosColision() const
{
    return m_eventosColision;
}

void Colisiones::avanzarPaso()
{
    m_tiempoActual += m_deltaTiempo;

    for (Particula &particula : m_particulas) {
        if (!particula.estaActiva()) {
            continue;
        }

        particula.mover(m_deltaTiempo);
        resolverColisionesPared(particula);
        resolverColisionesObstaculo(particula);
    }

    resolverColisionesParticulas();
    guardarPosicionesActuales();
}

void Colisiones::guardarPosicionesActuales()
{
    for (const Particula &particula : m_particulas) {
        if (!particula.estaActiva()) {
            continue;
        }

        m_trayectorias[particula.id()].append({m_tiempoActual,
                                               particula.posicion(),
                                               particula.velocidad(),
                                               particula.masa(),
                                               particula.radio()});
    }
}

void Colisiones::resolverColisionesPared(Particula &particula)
{
    Vector2D posicion = particula.posicion();
    Vector2D velocidad = particula.velocidad();
    bool colisiono = false;

    if (posicion.x - particula.radio() < m_limites.left()) {
        posicion.x = m_limites.left() + particula.radio();
        velocidad.x = std::abs(velocidad.x);
        colisiono = true;
    } else if (posicion.x + particula.radio() > m_limites.right()) {
        posicion.x = m_limites.right() - particula.radio();
        velocidad.x = -std::abs(velocidad.x);
        colisiono = true;
    }

    if (posicion.y - particula.radio() < m_limites.top()) {
        posicion.y = m_limites.top() + particula.radio();
        velocidad.y = std::abs(velocidad.y);
        colisiono = true;
    } else if (posicion.y + particula.radio() > m_limites.bottom()) {
        posicion.y = m_limites.bottom() - particula.radio();
        velocidad.y = -std::abs(velocidad.y);
        colisiono = true;
    }

    if (colisiono) {
        particula.setPosicion(posicion);
        particula.setVelocidad(velocidad);
        registrarColision(QString("Particula %1 reboto elasticamente contra una pared").arg(particula.id()));
    }
}

void Colisiones::resolverColisionesObstaculo(Particula &particula)
{
    for (const Obstaculo &obstaculo : m_obstaculos) {
        Vector2D normal;
        double penetracion = 0.0;

        if (!circuloIntersecaRectangulo(particula, obstaculo.rectangulo(), &normal, &penetracion)) {
            continue;
        }

        Vector2D velocidad = particula.velocidad();
        const double velocidadNormal = velocidad.productoPunto(normal);

        if (velocidadNormal < 0.0) {
            velocidad = velocidad - normal * ((1.0 + obstaculo.restitucion()) * velocidadNormal);
            particula.setVelocidad(velocidad);
        }

        particula.setPosicion(particula.posicion() + normal * penetracion);

        registrarColision(QString("Particula %1 choco inelasticamente con obstaculo %2 (e=%3)")
                              .arg(particula.id())
                              .arg(obstaculo.id())
                              .arg(obstaculo.restitucion(), 0, 'f', 2));
    }
}

void Colisiones::resolverColisionesParticulas()
{
    for (int i = 0; i < m_particulas.size(); ++i) {
        if (!m_particulas[i].estaActiva()) {
            continue;
        }

        for (int j = i + 1; j < m_particulas.size(); ++j) {
            if (!m_particulas[j].estaActiva()) {
                continue;
            }

            const Vector2D diferencia = m_particulas[j].posicion() - m_particulas[i].posicion();
            const double distanciaMinima = m_particulas[i].radio() + m_particulas[j].radio();

            if (diferencia.longitudCuadrada() > distanciaMinima * distanciaMinima) {
                continue;
            }

            const int idAbsorbido = m_particulas[j].id();
            m_particulas[i].absorber(m_particulas[j]);
            m_particulas[j].setActiva(false);

            registrarColision(QString("Particulas %1 y %2 se fusionaron completamente inelasticamente")
                                  .arg(m_particulas[i].id())
                                  .arg(idAbsorbido));
        }
    }
}

bool Colisiones::circuloIntersecaRectangulo(const Particula &particula,
                                            const QRectF &rectangulo,
                                            Vector2D *normal,
                                            double *penetracion) const
{
    const Vector2D centro = particula.posicion();
    const double xMasCercano = std::clamp(centro.x, rectangulo.left(), rectangulo.right());
    const double yMasCercano = std::clamp(centro.y, rectangulo.top(), rectangulo.bottom());

    Vector2D desdeCercano(centro.x - xMasCercano, centro.y - yMasCercano);
    const double distancia = desdeCercano.longitud();

    if (distancia > particula.radio()) {
        return false;
    }

    if (qFuzzyIsNull(distancia)) {
        const double profundidadIzquierda = std::abs(centro.x - rectangulo.left());
        const double profundidadDerecha = std::abs(rectangulo.right() - centro.x);
        const double profundidadSuperior = std::abs(centro.y - rectangulo.top());
        const double profundidadInferior = std::abs(rectangulo.bottom() - centro.y);

        const double minProfundidad = std::min(std::min(profundidadIzquierda, profundidadDerecha),
                                               std::min(profundidadSuperior, profundidadInferior));

        if (minProfundidad == profundidadIzquierda) {
            *normal = Vector2D(-1.0, 0.0);
            *penetracion = particula.radio() + profundidadIzquierda;
        } else if (minProfundidad == profundidadDerecha) {
            *normal = Vector2D(1.0, 0.0);
            *penetracion = particula.radio() + profundidadDerecha;
        } else if (minProfundidad == profundidadSuperior) {
            *normal = Vector2D(0.0, -1.0);
            *penetracion = particula.radio() + profundidadSuperior;
        } else {
            *normal = Vector2D(0.0, 1.0);
            *penetracion = particula.radio() + profundidadInferior;
        }

        return true;
    }

    *normal = desdeCercano / distancia;
    *penetracion = particula.radio() - distancia;
    return true;
}

void Colisiones::registrarColision(const QString &descripcion)
{
    m_eventosColision.append({m_tiempoActual, descripcion});
}