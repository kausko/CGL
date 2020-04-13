#include "a1pattern.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    A1Pattern w;
    w.show();
    return a.exec();
}
