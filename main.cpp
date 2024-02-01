#include <QApplication>
#include "fenetre.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    TransparentFrame fen;
    fen.showFullScreen();

    return app.exec();
}
