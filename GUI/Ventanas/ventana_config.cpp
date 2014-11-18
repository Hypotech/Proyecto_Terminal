#include "ventana_config.h"
#include "ui_ventana_config.h"

ventana_config::ventana_config(ReconocerdordePersona::metodo metdo, tamanyo tam, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventana_config)
{
    ui->setupUi(this);

    switch (metdo) {
    case ReconocerdordePersona::eigen:
        ui->Rdio_Eigenfaces->setChecked(true);
        break;
    case ReconocerdordePersona::fisher:
        ui->Rdio_Fisherfaces->setChecked(true);
        break;
    case ReconocerdordePersona::LBPH:
        ui->Rdio_LBPH->setChecked(true);
        break;
    }

    switch (tam) {
    case _200:
        ui->Rdio_200x200->setChecked(true);
        break;
    case _150:
        ui->Rdio_150x150->setChecked(true);
        break;
    case _100:
        ui->Rdio_100x100->setChecked(true);
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
    if(ui->Rdio_Eigenfaces->isChecked())
        modeloRecon = "eigen";
    else if(ui->Rdio_Fisherfaces->isChecked())
        modeloRecon = "fisher";
    else if(ui->Rdio_LBPH->isChecked())
        modeloRecon = "LBPH";

    if(ui->Rdio_100x100->isChecked())
        minRostro = 100;
    else if(ui->Rdio_150x150->isChecked())
        minRostro = 150;
    else if(ui->Rdio_200x200->isChecked())
        minRostro = 200;

    if ( GuardarConfig() ){
        QMessageBox::information(this,"Cambios guardados",
                             "Los cambios surtiran efecto hasta la próxima vez que inicie el programa");
        accept();
    }
    else{
        QMessageBox::warning(this,"Atención","Ocurrio un problema al intentar guardar el archivo \"config\"");
    }
}

bool ventana_config::GuardarConfig()
{
    fstream flujoArchivo("configuracion");

    if( flujoArchivo.is_open())
    {
        std::string linea;
        int flag = 0;

        streampos comienzoLinea = flujoArchivo.tellg(); //posicion donde comienza la linea
        while( std::getline(flujoArchivo,linea) && flag < 2 )
        {
            QStringList partes = QString(linea.c_str()).split("=", QString::SkipEmptyParts);

            if (partes.length() == 2){
                if( partes[0] == "minimoDetectar" ){
                    flujoArchivo.seekp(comienzoLinea); //se ecribira al inicio de la linea
                    flujoArchivo << partes[0].toStdString() << "=" << minRostro<< std::endl;
                    flag++;
                }
                else if( partes[0] == "modelo" ){
                    flujoArchivo.seekp(comienzoLinea); //se ecribira en el inicio de linea
                    flujoArchivo << partes[0].toStdString() << "=" << modeloRecon.toStdString()<< std::endl;
                    flag++;
//                    flujoArchivo.flush();
                }
            }
            comienzoLinea = flujoArchivo.tellg(); //posicion donde comienza la linea
        }
        flujoArchivo.close();
        return true;
    }
    flujoArchivo.close();
    return false;
}
