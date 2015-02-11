#include "serialterm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serialterm w;
    w.show();

    return a.exec();
}
