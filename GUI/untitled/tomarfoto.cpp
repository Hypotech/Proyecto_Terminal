#include "tomarfoto.h"
#include "ui_tomarfoto.h"

TomarFoto::TomarFoto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TomarFoto)
{
    ui->setupUi(this);
}

TomarFoto::~TomarFoto()
{
    delete ui;
}
