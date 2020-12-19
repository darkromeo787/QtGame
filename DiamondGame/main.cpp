#include <QApplication>
#include "diamondboard.h"

int main(int argc, char *argv[])
{
    diamond_game dgame;
    initGame(&dgame);

    initChessBoard(&(dgame.state));

    QApplication app(argc, argv);

    DiamondBoard board(&dgame);
    board.show();

    return app.exec();
}
