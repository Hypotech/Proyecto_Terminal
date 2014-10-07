#include "ventana_config.h"
#include "ui_ventana_config.h"

ventana_config::ventana_config(QWidget *parent, metodo metdo, tamanyo tam) :
    QDialog(parent),
    ui(new Ui::ventana_config)
{
    ui->setupUi(this);

    switch (metdo) {
    case eigen:
        ui->Rdio_Eigenfaces->setCheckable(true);
        break;
    case fisher:
        ui->Rdio_Fisherfaces->setCheckable(true);
        break;
    case LBPH:
        ui->Rdio_LBPH->setCheckable(true);
        break;
    }

    switch (tam) {
    case _200:
        ui->Rdio_200x200->setCheckable(true);
        break;
    case _150:
        ui->Rdio_150x150->setCheckable(true);
        break;
    case _100:
        ui->Rdio_100x100->setCheckable(true);
        break;
    }
}

ventana_config::~ventana_config()
{
    delete ui;
}

void ventana_config::on_Btn_Cancelar_clicked()
{
    reject();
}


void ventana_config::on_Btn_Aceptar_clicked()
{

    QMessageBox::information(this,"Cambios guardados",
                             "Los cambios surtiran efecto hasta la próxima vez que inicie el programa");
    accept();
}
