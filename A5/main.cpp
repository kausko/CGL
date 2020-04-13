#include "chessboard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessBoard c;
    c.show();
    return a.exec();
}
