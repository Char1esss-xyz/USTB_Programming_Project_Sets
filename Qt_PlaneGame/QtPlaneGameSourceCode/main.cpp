#include <QApplication>
#include "gameview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameView view;
    view.show();

    return app.exec();
}
