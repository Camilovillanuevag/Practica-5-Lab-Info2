#include "jugador.h"

#include "infraestructura.h"

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QtMath>
#include <algorithm>

Jugador::Jugador()
    : m_id(0),
    m_ladoIzquierdo(true),
    m_vidaRepresentante(100.0),
    m_representante(nullptr),
    m_textoVida(nullptr),
    m_canonGrupo(nullptr),
    m_canonTubo(nullptr)
{
}

Jugador::Jugador(int id, const QString &nombre, bool ladoIzquierdo)
    : m_id(id),
    m_nombre(nombre),
    m_ladoIzquierdo(ladoIzquierdo),
    m_vidaRepresentante(100.0),
    m_representante(nullptr),
    m_textoVida(nullptr),
    m_canonGrupo(nullptr),
    m_canonTubo(nullptr)
{
}

int Jugador::id() const
{
    return m_id;
}

QString Jugador::nombre() const
{
    return m_nombre;
}

bool Jugador::ladoIzquierdo() const
{
    return m_ladoIzquierdo;
}

double Jugador::vidaRepresentante() const
{
    return m_vidaRepresentante;
}

QPointF Jugador::puntoLanzamiento() const
{
    return m_puntoLanzamiento;
}

QVector<Infraestructura *> Jugador::infraestructura() const
{
    return m_infraestructura;
}

bool Jugador::infraestructuraDestruida() const
{
    for (Infraestructura *item : m_infraestructura) {
        if (item && !item->destruida()) {
            return false;
        }
    }
    return true;
}

bool Jugador::representanteDerrotado() const
{
    return m_vidaRepresentante <= 0.0;
}

QGraphicsEllipseItem *Jugador::representanteItem() const
{
    return m_representante;
}

void Jugador::configurarGraficos(QGraphicsScene *scene, const QRectF &caja)
{
    const double baseY = caja.bottom() - 150.0;
    const double centroRepresentanteX = m_ladoIzquierdo ? caja.left() + 205.0 : caja.right() - 205.0;
    const double canonX = m_ladoIzquierdo ? caja.left() + 45.0 : caja.right() - 45.0;
    const double canonY = caja.top() + 320.0;

    m_puntoLanzamiento = QPointF(canonX, canonY);

    m_representante = scene->addEllipse(-18.0, -18.0, 36.0, 36.0,
                                        QPen(QColor("#1f2937"), 2.0),
                                        QBrush(m_ladoIzquierdo ? QColor("#2b6cb0") : QColor("#c53030")));
    m_representante->setPos(centroRepresentanteX, baseY + 60.0);
    m_representante->setZValue(2.0);

    m_textoVida = scene->addText("");
    QFont fuente;
    fuente.setBold(true);
    fuente.setPointSize(9);
    m_textoVida->setFont(fuente);
    m_textoVida->setDefaultTextColor(QColor("#111827"));
    actualizarTexto();

    // Canon dibujado solo con figuras basicas de QGraphicsScene.
    m_canonGrupo = new QGraphicsItemGroup();
    scene->addItem(m_canonGrupo);

    QGraphicsEllipseItem *rueda = new QGraphicsEllipseItem(-12.0, -12.0, 24.0, 24.0, m_canonGrupo);
    rueda->setBrush(QBrush(QColor("#374151")));
    rueda->setPen(QPen(QColor("#111827"), 1.5));

    QGraphicsRectItem *base = new QGraphicsRectItem(-18.0, 7.0, 36.0, 10.0, m_canonGrupo);
    base->setBrush(QBrush(QColor("#6b7280")));
    base->setPen(QPen(QColor("#111827"), 1.5));

    m_canonTubo = new QGraphicsLineItem(0.0, 0.0, m_ladoIzquierdo ? 45.0 : -45.0, -10.0, m_canonGrupo);
    m_canonTubo->setPen(QPen(QColor("#111827"), 7.0, Qt::SolidLine, Qt::RoundCap));

    m_canonGrupo->setPos(m_puntoLanzamiento);
    m_canonGrupo->setZValue(5.0);
}

void Jugador::agregarInfraestructura(Infraestructura *item)
{
    if (item) {
        m_infraestructura.append(item);
    }
}

void Jugador::aplicarDanoRepresentante(double dano)
{
    m_vidaRepresentante = std::max(0.0, m_vidaRepresentante - dano);
    actualizarTexto();
}

void Jugador::actualizarCanon(double anguloGrados)
{
    if (!m_canonTubo) {
        return;
    }

    const double rad = qDegreesToRadians(anguloGrados);
    const double largo = 48.0;
    const double direccionX = m_ladoIzquierdo ? 1.0 : -1.0;
    const double x2 = direccionX * largo * std::cos(rad);
    const double y2 = -largo * std::sin(rad);
    m_canonTubo->setLine(0.0, 0.0, x2, y2);
}

void Jugador::actualizarTexto()
{
    if (!m_textoVida || !m_representante) {
        return;
    }

    const QString vulnerable = infraestructuraDestruida() ? "  Vulnerable" : "";
    m_textoVida->setPlainText(QString("%1\nVida: %2%3")
                                  .arg(m_nombre)
                                  .arg(static_cast<int>(m_vidaRepresentante + 0.5))
                                  .arg(vulnerable));
    const QRectF textoRect = m_textoVida->boundingRect();
    m_textoVida->setPos(m_representante->pos().x() - textoRect.width() / 2.0,
                        m_representante->pos().y() + 24.0);
}

