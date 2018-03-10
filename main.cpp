#include "overview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Overview w;
    w.show();

    return a.exec();
}
