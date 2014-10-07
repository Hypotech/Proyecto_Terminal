#include "agregar_usur_foto.h"
#include "ui_agregar_usur_foto.h"

agregar_usur_Foto::agregar_usur_Foto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::agregar_usur_Foto)
{
    ui->setupUi(this);

    ui->comBox_CamarasList->addItem("Raspicam");
    ui->lview_Capturas->setViewMode(QListView::IconMode);
    ui->lview_Capturas->setMovement(QListView::Static);
    ui->lview_Capturas->setIconSize(QSize(100,120));
    modelo = new QStandardItemModel;
    ui->lview_Capturas->setModel(modelo);

    NumDeFotos = 0;

    QCamera camara;
    foreach(const QByteArray &NumeroCamara, QCamera::availableDevices()) { //camaras conectadas directamente al Servidor
//        IDscamLocal.push_back(NumeroCamara);
        QString NombreCamara = camara.deviceDescription(NumeroCamara);
        ui->comBox_CamarasList->addItem(NombreCamara);
    }

    connect(Temporizador,SIGNAL(timeout()),SLOT(SL_Desplegar_camaraLocal));
    connect( ui->comBox_CamarasList,SIGNAL( currentIndexChanged(int) ),SLOT( SL_cambiarDeCamara(int) ) );//seleccion de camara
    connect( this,SIGNAL(CambioValorNumDeFotos()),SLOT( SL_Inhabilitar_Botones() ) ); //sólo necesarias 8 fotos
    connect( ui->lview_Capturas->selectionModel(),SIGNAL( selectionChanged (QItemSelection,QItemSelection) ),
            this, SLOT( SL_Habilitar_Btn_EliminarFoto(QItemSelection) ) );

    SL_cambiarDeCamara(0); //raspicam

}

agregar_usur_Foto::~agregar_usur_Foto()
{
    delete ui;
    delete camaraLocal;
    delete Temporizador;
    delete modelo;
}

void agregar_usur_Foto::on_Btn_TomarFoto_clicked()
{
    NumDeFotos++;
    foto.push_back(RostroDetectado);

    QPixmap Pixtemporal = convertir_Mat_a_Pixmap(RostroDetectado);
    QStandardItem* nuevaFoto = new QStandardItem( QIcon(Pixtemporal), QString::number(NumDeFotos) );
    capturas.push_back(nuevaFoto);

    modelo->appendRow(nuevaFoto);
    ui->lview_Capturas->scrollToBottom(); //scroll hasta el ultima captura

    emit CambioValorNumDeFotos();
}

bool agregar_usur_Foto::SL_cambiarDeCamara(int IndiceComboBox)
{
    delete camaraLocal;

    if(IndiceComboBox == 0){
        Temporizador->stop(); //detiene el timer
        connect( this, SIGNAL(RaspicamLista(cv::Mat&)),SLOT(SL_Desplegar_Raspicam(cv::Mat&)) );
        connect(this, SIGNAL(RaspicamRostroDectado(cv::Mat&)),SLOT(SL_RostroDetectado(cv::Mat&)) );
        connect(this,SIGNAL(RaspicamRostroNODectado()),SLOT(SL_RostroNODetectado()));
        return true;
    }
    else if(IndiceComboBox > 0){
        disconnect(this, SIGNAL(RaspicamLista(cv::Mat&)),this,SLOT(SL_Desplegar_Raspicam(cv::Mat&)) );
        disconnect(this, SIGNAL(RaspicamRostroDectado(cv::Mat&)), this,SLOT(SL_RostroDetectado(cv::Mat&)) );
        disconnect(this,SIGNAL(RaspicamRostroNODectado()),this,SLOT(SL_RostroNODetectado()));
        camaraLocal = new Camara(IndiceComboBox-1);// abrimos la camara con el id especificado
        Temporizador->start(1000/25); //empieza a correr el timer
        return true;
    }

    return false;
}

void agregar_usur_Foto::SL_Desplegar_camaraLocal()
{
    cv::Mat Imagen;

    camaraLocal->GetFrame(Imagen);//lee un cuadro desde la camara

    ui->lbl_ImageDisplay->setPixmap( convertir_Mat_a_Pixmap(Imagen) );
}

void agregar_usur_Foto::SL_Desplegar_Raspicam(Mat& Imagen)
{
    ui->lbl_ImageDisplay->setPixmap( convertir_Mat_a_Pixmap(Imagen) );
}

void agregar_usur_Foto::SL_Inhabilitar_Botones()
{
    if(NumDeFotos == 8){
        ui->Btn_TomarFoto->setEnabled(false);
        ui->Btn_Continuar->setEnabled(true);
    }
    else{
        ui->Btn_TomarFoto->setEnabled(true);
        ui->Btn_Continuar->setEnabled(false);
    }

    if(NumDeFotos == 0)
        ui->Btn_Eliminar->setEnabled(false);
    else
        ui->Btn_Eliminar->setEnabled(true);
}

QPixmap agregar_usur_Foto::convertir_Mat_a_Pixmap(Mat& ImagenEntrada)
{
    if(ImagenEntrada.type()==CV_8UC1)
    {
        QImage imagenQT(
                    (const uchar*)ImagenEntrada.data,
                    ImagenEntrada.cols,
                    ImagenEntrada.rows,
                    ImagenEntrada.step,
                    QImage::Format_Indexed8);

        imagenQT.setColorTable(TablaDColores);
        return QPixmap::fromImage(imagenQT);
    }

    else if(ImagenEntrada.type()==CV_8UC3)
    {
        QImage imagenQT(
                    (const unsigned char*)(ImagenEntrada.data),
                    ImagenEntrada.cols,
                    ImagenEntrada.rows,
                    ImagenEntrada.step,
                    QImage::Format_RGB888 );

        imagenQT = imagenQT.rgbSwapped();
        return QPixmap::fromImage(imagenQT);
    }
}


void agregar_usur_Foto::on_Btn_Eliminar_clicked()
{
    int ElemenSelec = ui->lview_Capturas->currentIndex().row();
    modelo->takeItem(ElemenSelec); //lo quita de la ListView
    QStandardItem* ElemenEliminar = capturas.takeAt(ElemenSelec);
    delete ElemenEliminar; //retorna la memoria ocupada

    foto.takeAt(ElemenSelec);
    --NumDeFotos;
    emit CambioValorNumDeFotos();
}

void agregar_usur_Foto::SL_Habilitar_Btn_EliminarFoto(QItemSelection)
{
    ui->Btn_Eliminar->setEnabled(true);
}

void agregar_usur_Foto::SL_RostroDetectado(Mat& Rostro)
{
    RostroDetectado = Rostro;
    ui->Btn_TomarFoto->setEnabled(true);
}

void agregar_usur_Foto::SL_RostroNODetectado()
{
    ui->Btn_TomarFoto->setEnabled(false);
}
