#include "hashlife.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hashlife w;
    w.show();
    return a.exec();
}
