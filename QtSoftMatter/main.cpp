#include "qtsoftmatter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSoftMatter w;
    w.show();

    return a.exec();
}
