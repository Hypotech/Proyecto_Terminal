#include "agregar_usuario.h"
#include "ui_agregar_usuario.h"

Agregar_Usuario::Agregar_Usuario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Agregar_Usuario)
{
    ui->setupUi(this);
}

Agregar_Usuario::~Agregar_Usuario()
{
    delete ui;
}

void Agregar_Usuario::on_Btn_Continuar_clicked()
{

    accept();
}

void Agregar_Usuario::on_Btn_Cancelar_clicked()
{
    reject();
}
