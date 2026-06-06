#include "juegoestrategico.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JuegoEstrategico ventana;
    ventana.show();

    return app.exec();
}

