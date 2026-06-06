#include "infraestructura.h"

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QPen>
#include <algorithm>

Infraestructura::Infraestructura(int propietario,
                                 const QString &nombre,
                                 const QRectF &rectanguloEscena,
                                 double resistenciaInicial,
                                 double restitucion,
                                 QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      m_propietario(propietario),
      m_nombre(nombre),
      m_resistencia(resistenciaInicial),
      m_resistenciaInicial(resistenciaInicial),
      m_restitucion(restitucion),
      m_texto(nullptr)
{
    setRect(rectanguloEscena);
    setBrush(QBrush(QColor("#ffe8a3")));
    setPen(QPen(QColor("#b7791f"), 2.0));
    setZValue(3.0);

    m_texto = new QGraphicsTextItem(this);
    m_texto->setDefaultTextColor(QColor("#3b2f1e"));
    QFont fuente;
    fuente.setBold(true);
    fuente.setPointSize(9);
    m_texto->setFont(fuente);
    actualizarTexto();
}

int Infraestructura::propietario() const
{
    return m_propietario;
}

QString Infraestructura::nombre() const
{
    return m_nombre;
}

double Infraestructura::resistencia() const
{
    return m_resistencia;
}

double Infraestructura::restitucion() const
{
    return m_restitucion;
}

bool Infraestructura::destruida() const
{
    return m_resistencia <= 0.0;
}

void Infraestructura::aplicarDano(double dano)
{
    m_resistencia = std::max(0.0, m_resistencia - dano);
    if (destruida()) {
        setBrush(QBrush(QColor("#d9d9d9")));
        setPen(QPen(QColor("#666666"), 1.2, Qt::DashLine));
        setOpacity(0.35);
    }
    actualizarTexto();
}

void Infraestructura::actualizarTexto()
{
    if (!m_texto) {
        return;
    }

    if (destruida()) {
        m_texto->setPlainText("0");
    } else {
        m_texto->setPlainText(QString::number(static_cast<int>(m_resistencia + 0.5)));
    }

    const QRectF r = rect();
    const QRectF t = m_texto->boundingRect();
    m_texto->setPos(r.center().x() - t.width() / 2.0,
                    r.center().y() - t.height() / 2.0);
}

QRectF Infraestructura::rectanguloEscena() const
{
    return sceneBoundingRect();
}
