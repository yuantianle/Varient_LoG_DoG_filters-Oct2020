#include "Mark.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mark w;
    w.show();
    return a.exec();
}
