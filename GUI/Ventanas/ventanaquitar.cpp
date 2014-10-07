#include "ventanaquitar.h"
#include "ui_ventanaquitar.h"

ventanaQuitar::ventanaQuitar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaQuitar)
{
    ui->setupUi(this);
}

ventanaQuitar::~ventanaQuitar()
{
    delete ui;
}
