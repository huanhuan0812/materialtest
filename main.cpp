#include <QApplication>
#include "QtMaterial/global/init.hpp"
#include "widget.h"
#include "QtMaterial/QtMaterial"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    material::initTheme();

    Widget w;
    w.show();

    return a.exec();
}
