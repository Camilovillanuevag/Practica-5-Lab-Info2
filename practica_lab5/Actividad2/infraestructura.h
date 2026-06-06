#ifndef INFRAESTRUCTURA_H
#define INFRAESTRUCTURA_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QRectF>
#include <QString>

class Infraestructura : public QGraphicsRectItem
{
public:
    Infraestructura(int propietario,
                    const QString &nombre,
                    const QRectF &rectanguloEscena,
                    double resistenciaInicial,
                    double restitucion,
                    QGraphicsItem *parent = nullptr);

    int propietario() const;
    QString nombre() const;
    double resistencia() const;
    double restitucion() const;
    bool destruida() const;

    void aplicarDano(double dano);
    void actualizarTexto();
    QRectF rectanguloEscena() const;

private:
    int m_propietario;
    QString m_nombre;
    double m_resistencia;
    double m_resistenciaInicial;
    double m_restitucion;
    QGraphicsTextItem *m_texto;
};

#endif
