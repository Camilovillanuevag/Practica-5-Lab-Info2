#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QString>
#include <QVector>

class Infraestructura;

class Jugador
{
public:
    Jugador();
    Jugador(int id, const QString &nombre, bool ladoIzquierdo);

    int id() const;
    QString nombre() const;
    bool ladoIzquierdo() const;
    double vidaRepresentante() const;
    QPointF puntoLanzamiento() const;
    QVector<Infraestructura *> infraestructura() const;
    bool infraestructuraDestruida() const;
    bool representanteDerrotado() const;
    QGraphicsEllipseItem *representanteItem() const;

    void configurarGraficos(QGraphicsScene *scene, const QRectF &caja);
    void agregarInfraestructura(Infraestructura *item);
    void aplicarDanoRepresentante(double dano);
    void actualizarCanon(double anguloGrados);
    void actualizarTexto();

private:
    int m_id;
    QString m_nombre;
    bool m_ladoIzquierdo;
    double m_vidaRepresentante;
    QPointF m_puntoLanzamiento;
    QVector<Infraestructura *> m_infraestructura;

    QGraphicsEllipseItem *m_representante;
    QGraphicsTextItem *m_textoVida;
    QGraphicsItemGroup *m_canonGrupo;
    QGraphicsLineItem *m_canonTubo;
};

#endif // JUGADOR_H
