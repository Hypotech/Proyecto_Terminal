#include "tomarfoto.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TomarFoto w;
    w.show();

    return a.exec();
}
