#include "trayectoria.h"

#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <algorithm>

TrayectoriaWidget::TrayectoriaWidget(const Colisiones &colisiones, QWidget *parent)
    : QWidget(parent),
    m_colisiones(colisiones),
    m_frameActual(0)
{
    setWindowTitle("Actividad 1 - Trayectorias de partículas");
    resize(1000, 700);

    connect(&m_timer, &QTimer::timeout, this, &TrayectoriaWidget::avanzarAnimacion);
    m_timer.start(16);
}

bool TrayectoriaWidget::guardarImagen(const QString &rutaArchivo, const QSize &tamanoImagen)
{
    QImage image(tamanoImagen, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor("#f8f9fa"));

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    dibujarSimulacion(painter,
                      QRectF(50.0, 50.0, tamanoImagen.width() - 100.0, tamanoImagen.height() - 110.0),
                      cantidadMaximaDeFrames() - 1);
    return image.save(rutaArchivo);
}

void TrayectoriaWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor("#f8f9fa"));
    dibujarSimulacion(painter, QRectF(45.0, 45.0, width() - 90.0, height() - 95.0), m_frameActual);
}

QPointF TrayectoriaWidget::convertirAScreen(const Vector2D &punto, const QRectF &lienzo) const
{
    const QRectF limites = m_colisiones.limites();
    const double escalaX = lienzo.width() / limites.width();
    const double escalaY = lienzo.height() / limites.height();

    return QPointF(lienzo.left() + (punto.x - limites.left()) * escalaX,
                   lienzo.top() + (punto.y - limites.top()) * escalaY);
}

void TrayectoriaWidget::dibujarSimulacion(QPainter &painter, const QRectF &lienzo, int frame)
{
    const QRectF limites = m_colisiones.limites();
    const double escalaX = lienzo.width() / limites.width();
    const double escalaY = lienzo.height() / limites.height();

    painter.setPen(QPen(QColor("#1f2937"), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(lienzo);

    painter.setPen(QPen(QColor("#94a3b8"), 1, Qt::DashLine));
    for (int x = 100; x < limites.width(); x += 100) {
        const double screenX = lienzo.left() + x * escalaX;
        painter.drawLine(QPointF(screenX, lienzo.top()), QPointF(screenX, lienzo.bottom()));
    }
    for (int y = 100; y < limites.height(); y += 100) {
        const double screenY = lienzo.top() + y * escalaY;
        painter.drawLine(QPointF(lienzo.left(), screenY), QPointF(lienzo.right(), screenY));
    }

    painter.setPen(QPen(QColor("#111827"), 1));
    painter.setBrush(QColor("#4b5563"));
    for (const Obstaculo &obstaculo : m_colisiones.obstaculos()) {
        const QRectF rectangulo = obstaculo.rectangulo();
        QRectF rectanguloPantalla(lienzo.left() + rectangulo.left() * escalaX,
                                  lienzo.top() + rectangulo.top() * escalaY,
                                  rectangulo.width() * escalaX,
                                  rectangulo.height() * escalaY);
        painter.drawRect(rectanguloPantalla);
    }

    const QVector<Particula> particulas = m_colisiones.particulas();
    const QMap<int, QVector<PuntoTrayectoria>> trayectorias = m_colisiones.trayectorias();

    for (const Particula &particula : particulas) {
        const QVector<PuntoTrayectoria> puntos = trayectorias.value(particula.id());
        if (puntos.size() < 2) {
            continue;
        }

        const int ultimoIndice = std::min(frame, static_cast<int>(puntos.size()) - 1);
        if (ultimoIndice <= 0) {
            continue;
        }

        QPainterPath path;
        path.moveTo(convertirAScreen(puntos.first().posicion, lienzo));
        for (int i = 1; i <= ultimoIndice; ++i) {
            path.lineTo(convertirAScreen(puntos[i].posicion, lienzo));
        }

        painter.setPen(QPen(particula.color(), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);

        painter.setPen(QPen(QColor("#111827"), 1));
        painter.setBrush(particula.color());
        const QPointF puntoActual = convertirAScreen(puntos[ultimoIndice].posicion, lienzo);
        const double radio = std::max(4.0, puntos[ultimoIndice].radio * (escalaX + escalaY) * 0.5);
        painter.drawEllipse(puntoActual, radio, radio);
    }
}

int TrayectoriaWidget::cantidadMaximaDeFrames() const
{
    int maximo = 0;
    const QMap<int, QVector<PuntoTrayectoria>> trayectorias = m_colisiones.trayectorias();
    for (const QVector<PuntoTrayectoria> &puntos : trayectorias) {
        maximo = std::max(maximo, static_cast<int>(puntos.size()));
    }
    return maximo;
}

void TrayectoriaWidget::avanzarAnimacion()
{
    const int maximo = cantidadMaximaDeFrames();
    if (maximo <= 1) {
        return;
    }

    m_frameActual++;
    if (m_frameActual >= maximo) {
        m_frameActual = 0;
    }

    update();
}
