#include "ventanaquitar.h"
#include "ui_ventanaquitar.h"

ventanaQuitar::ventanaQuitar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaQuitar)
{
    ui->setupUi(this);

    ui->lv_displayUsuarios->setViewMode(QListView::IconMode);
    ui->lv_displayUsuarios->setMovement(QListView::Static);
    ui->lv_displayUsuarios->setIconSize(QSize(100,120));
    modelo = new QStandardItemModel;
    ui->lv_displayUsuarios->setModel(modelo);

    std::vector<std::string> listaCarpetas = manejoArchivos::listarCarpeta("BD");

    for(std::vector <std::string> ::const_iterator i = listaCarpetas.begin();
        i != listaCarpetas.end(); i++){ //se cargan una imagen para mostrar, de cada usuario

        std::vector<std::string> listaArchivos = manejoArchivos::listarArchivos("BD/" + *i );
//        cv::Mat matTemporal = cv::imread(*i,0);
//        QPixmap* Pixtemporal = new QPixmap (convertir_Mat_a_Pixmap(matTemporal));
        if (listaArchivos.empty() == false){

            std::string ruta("BD/" + *i + "/" + listaArchivos.at(0));
            QIcon* usuarFoto = new QIcon ( QString(ruta.c_str()) );

            QStandardItem* fotoUsurio = new QStandardItem( *usuarFoto, QString(i->c_str()) );
            modelo->appendRow(fotoUsurio);
            listaFotosUsuarios.push_back(fotoUsurio);
        }
    }

    connect( ui->lv_displayUsuarios->selectionModel(),
            SIGNAL( selectionChanged (QItemSelection,QItemSelection) ),
            this, SLOT( SL_Habilitar_Btn_EliminarFoto() ) );
}

ventanaQuitar::~ventanaQuitar()
{
    delete ui;
}

void ventanaQuitar::SL_Habilitar_Btn_EliminarFoto()
{
    ui->Btn_Eliminar->setEnabled(true);
}

void ventanaQuitar::on_Btn_Eliminar_clicked()
{
    int ElemenSelec = ui->lv_displayUsuarios->currentIndex().row();
        QStandardItem* ElemenEliminar = listaFotosUsuarios[ElemenSelec];
    if( QMessageBox::question(this,"Confirmar acción",
                              "¿Está seguro de eliminar el usuario " + ElemenEliminar->text() + "?",
                              QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ){

        listaFotosUsuarios.takeAt(ElemenSelec);
        manejoArchivos::quitarCarpeta("BD/" + ElemenEliminar->text().toStdString()); //elimina carpeta
        ui->lv_displayUsuarios->model()->removeRow(ElemenSelec); //lo quita de la ListView
    }
}
