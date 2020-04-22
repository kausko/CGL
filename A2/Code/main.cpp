#include "a2pattern.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    A2Pattern w;
    w.show();
    return a.exec();
}
