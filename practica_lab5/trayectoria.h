#ifndef TRAYECTORIA_H
#define TRAYECTORIA_H

#include "colisiones.h"

#include <QTimer>
#include <QWidget>

class TrayectoriaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrayectoriaWidget(const Colisiones &colisiones, QWidget *parent = nullptr);

    bool guardarImagen(const QString &rutaArchivo, const QSize &tamanoImagen = QSize(1000, 700));

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPointF convertirAScreen(const Vector2D &punto, const QRectF &lienzo) const;
    void dibujarSimulacion(QPainter &painter, const QRectF &lienzo, int frame);
    int cantidadMaximaDeFrames() const;
    void avanzarAnimacion();

    Colisiones m_colisiones;
    QTimer m_timer;
    int m_frameActual;
};

#endif
