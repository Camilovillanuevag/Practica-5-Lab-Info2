# Diagrama de clases - Actividad 2

```mermaid
classDiagram
    class JuegoEstrategico {
        -QGraphicsScene* m_escena
        -QGraphicsView* m_vista
        -Jugador m_jugador1
        -Jugador m_jugador2
        -Proyectil* m_proyectil
        -QTimer m_timer
        -int m_turno
        -double m_gravedad
        -double m_factorDanoInfraestructura
        -double m_factorDanoRepresentante
        +lanzarProyectil()
        +actualizarFisica()
        +reiniciarJuego()
        -revisarColisionesInfraestructura()
        -revisarColisionRepresentante()
        -finalizarTurno(QString)
    }

    class Jugador {
        -int m_id
        -QString m_nombre
        -bool m_ladoIzquierdo
        -double m_vidaRepresentante
        -QVector~Infraestructura*~ m_infraestructura
        -QGraphicsEllipseItem* m_representante
        -QGraphicsLineItem* m_canonTubo
        +configurarGraficos(QGraphicsScene*, QRectF)
        +aplicarDanoRepresentante(double)
        +actualizarCanon(double)
        +infraestructuraDestruida() bool
    }

    class Infraestructura {
        -int m_propietario
        -QString m_nombre
        -double m_resistencia
        -double m_restitucion
        -QGraphicsTextItem* m_texto
        +aplicarDano(double)
        +destruida() bool
        +rectanguloEscena() QRectF
    }

    class Proyectil {
        -Vector2D m_posicion
        -Vector2D m_velocidad
        -double m_masa
        -double m_radio
        -bool m_activo
        +avanzar(double, double)
        +resolverChoqueConParedes(QRectF)
        +rebotarConNormal(Vector2D, double)
        +rapidez() double
    }

    class Vector2D {
        +double x
        +double y
        +longitud() double
        +normalizado() Vector2D
        +productoPunto(Vector2D) double
    }

    JuegoEstrategico --> Jugador
    JuegoEstrategico --> Proyectil
    JuegoEstrategico --> Infraestructura
    Jugador --> Infraestructura
    Proyectil --> Vector2D
```

## Relaciones

- `JuegoEstrategico` administra toda la simulacion: escena, turnos, controles, proyectil activo y reglas de victoria.
- Cada `Jugador` posee su representante, su canon y una lista de objetos `Infraestructura`.
- `Infraestructura` hereda de `QGraphicsRectItem`, por eso se puede dibujar directamente en la escena.
- `Proyectil` hereda de `QGraphicsEllipseItem` y ademas mantiene su estado fisico interno.
- `Vector2D` evita mezclar calculos fisicos con objetos graficos de Qt.
