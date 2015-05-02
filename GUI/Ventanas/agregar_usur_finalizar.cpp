#include "agregar_usur_finalizar.h"
#include "ui_agregar_usur_finalizar.h"

agregar_usur_finalizar::agregar_usur_finalizar(QVector<cv::Mat>& Rostros, QWidget *parent) :
    QDialog(parent),
        rostrosEntrada(Rostros),
    ui(new Ui::agregar_usur_finalizar)
{
    ui->setupUi(this);

    numFotos = 0;
    ui->Btn_Finalizar->setVisible(false);

    std::vector<std::string> lista = manejoArchivos::listarCarpeta("BD");
    if (lista.empty())
        almacenCarpeta = "s1";
    else{
        int numCarpeta = std::stoi(lista.back().substr(1)) + 1;
        almacenCarpeta = "s" + std::to_string(numCarpeta); //almacenar la ultima + 1
    }

    compresion_param.push_back(CV_IMWRITE_PXM_BINARY);
    compresion_param.push_back(1);

    for (int i=0; i<256; i++)
        TablaDColores.push_back(qRgb(i,i,i));

    ui->Lbl_imagenDisplay->installEventFilter(this);

    encotrarOjos( rostrosEntrada[0], RostroOjosDetec);

    ui->Lbl_imagenDisplay->setPixmap(convertir_Mat_a_Pixmap(RostroOjosDetec));
    ui->LEdit_ojoIzq->setText( QString::number(ojoIzq.x) +","+ QString::number(ojoIzq.y) );
    ui->LEdit_ojoDer->setText( QString::number(ojoDer.x) +","+ QString::number(ojoDer.y) );

    connect(this,SIGNAL(cambioValornumFotos()),SLOT(SL_InhabilitarBotones()));

    statusBar = new QStatusBar;
    ui->vl_StatusBar->addWidget(statusBar);
}

agregar_usur_finalizar::~agregar_usur_finalizar()
{
    delete ui;
}

bool agregar_usur_finalizar::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    statusBar->showMessage(
                QString("Coordenadas: (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()) );
    return true;
  }
  if(event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if( mouseEvent->button() == Qt::LeftButton ){
          ui->LEdit_ojoIzq->setText(
                      QString("%1,%2").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()) );
          ojoIzq = cv::Point ( mouseEvent->pos().x(), mouseEvent->pos().y() );
      }
      else if(mouseEvent->button() == Qt::RightButton){
          ui->LEdit_ojoDer->setText(
                      QString("%1,%2").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()) );
          ojoDer = cv::Point ( mouseEvent->pos().x(), mouseEvent->pos().y() );
      }
  }
  return false;
}

QPixmap agregar_usur_finalizar::convertir_Mat_a_Pixmap(cv::Mat& ImagenEntrada)
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
    return QPixmap();
}

void agregar_usur_finalizar::on_Btn_Recortar_clicked()
{
    QProcess* process = new QProcess(this);
    QString rutaAlmacen = QString("/tmp/%1.pgm").arg(numFotos);

    if ( cv::imwrite(rutaAlmacen.toStdString(), rostrosEntrada[numFotos], compresion_param) == false){
        QMessageBox::warning(this,"Sin exito","Fallo al intentar alamecanar la imagen");
        return;
    }

    process->start( "./Utilidades/alignement.py",
                  QStringList() << QString::number(ojoIzq.x) << QString::number(ojoIzq.y) <<
                   QString::number(ojoDer.x) << QString::number(ojoDer.y) <<
                    QString::number(ui->douSB_offset->value()) << QString::number(ui->douSB_offset->value())
                  << rutaAlmacen);
    sleep(2);
    imagenRecortada = cv::imread(rutaAlmacen.toStdString(),B_N);

    ui->Lbl_imagenDisplay->setPixmap(convertir_Mat_a_Pixmap(imagenRecortada));

    ui->Btn_Recortar->setEnabled(false);
    ui->Btn_Deshacer->setEnabled(true);

    if(numFotos == 7)
        ui->Btn_Finalizar->setEnabled(true);
    else
        ui->Btn_Siguiente->setEnabled(true);
}

void agregar_usur_finalizar::on_Btn_Siguiente_clicked()
{
        imagenes_a_Guardar.push_back(imagenRecortada);
        numFotos++;

        encotrarOjos( rostrosEntrada[numFotos],RostroOjosDetec );
        ui->Lbl_imagenDisplay->setPixmap(convertir_Mat_a_Pixmap(RostroOjosDetec));
        ui->LEdit_ojoIzq->setText( QString::number(ojoIzq.x) +","+ QString::number(ojoIzq.y) );
        ui->LEdit_ojoDer->setText( QString::number(ojoDer.x) +","+ QString::number(ojoDer.y) );

        emit cambioValornumFotos();
        ui->Btn_Recortar->setEnabled(true);
        ui->Btn_Deshacer->setEnabled(false);
        ui->Btn_Siguiente->setEnabled(false);
        ui->Lbl_numImagen->setText(QString("%1/8").arg(numFotos+1));
}

bool agregar_usur_finalizar::almacenarImagen()
{
    if( manejoArchivos::crearCarpeta("BD/" + almacenCarpeta + "/") == false)
            return false;

        int conteoFoto = 1;

        for( QVector<cv::Mat>::const_iterator i = imagenes_a_Guardar.begin();
            i != imagenes_a_Guardar.end(); i++, conteoFoto++ ){
            if ( cv::imwrite("BD/" + almacenCarpeta + "/" + QString::number(conteoFoto).toStdString() + ".pgm",
                        *i, compresion_param) == false )
                return false;
        }
        return true;
}

void agregar_usur_finalizar::on_Btn_Finalizar_clicked()
{
    if ( almacenarImagen() == false ){
        manejoArchivos::quitarCarpeta("BD/" + almacenCarpeta);
        QMessageBox::warning(this,"Error al almacenar",
                             "Ocurrio un error al intertar alamacenar las imagenes. No se agrego el usuario");
        reject();
    }
    else{
        QMessageBox::information(this,"Usuario agregado",
                             "Los cambios surtiran efecto hasta la próxima vez que inicie el programa");
        accept();
    }
}

void agregar_usur_finalizar::SL_InhabilitarBotones()
{
    if (numFotos < 7){
        ui->Btn_Finalizar->setVisible(false);
        ui->Btn_Siguiente->setVisible(true);
    }

    else{
        ui->Btn_Finalizar->setVisible(true);
        ui->Btn_Siguiente->setVisible(false);
    }
}

void agregar_usur_finalizar::on_Btn_Deshacer_clicked()
{
    ui->Btn_Recortar->setEnabled(true);
    ui->Btn_Deshacer->setEnabled(false);
    ui->Lbl_imagenDisplay->setPixmap(convertir_Mat_a_Pixmap(RostroOjosDetec));
}

void agregar_usur_finalizar::encotrarOjos(cv::Mat &iEntrada, cv::Mat& iSalida)
{
    iEntrada.copyTo(iSalida);
    std::vector <cv::Rect> ubiciacion;

    detectorOjos.EscanearImagen(iSalida,ubiciacion);

    if (ubiciacion.size() > 1){
        cv::Scalar Blanco (255,255,255);

        cv::Point ojosCoordenas[2];
        int i = 0;
        for ( std::vector<cv::Rect>::const_iterator ojo = ubiciacion.begin(); i < 2; ojo++, i++ ){

            cv::rectangle( iSalida, *ojo, Blanco);
            cv::Rect rectangulo= *ojo;

            cv::line(iSalida,rectangulo.tl(),rectangulo.br(),Blanco);
            cv::line(iSalida,cv::Point(rectangulo.x + rectangulo.width,rectangulo.y),
                     cv::Point(rectangulo.x,rectangulo.y + rectangulo.height),Blanco);

            ojosCoordenas[i] = cv::Point (rectangulo.x + rectangulo.width/2,
                                          rectangulo.y + rectangulo.height/2 );
        }

        if (ojosCoordenas[0].x < ojosCoordenas[1].x){ //se determina quien está más a la izquierda
            ojoIzq = ojosCoordenas[0];
            ojoDer = ojosCoordenas[1];
        }
        else{
            ojoIzq = ojosCoordenas[1];
            ojoDer = ojosCoordenas[0];
        }

    }

}
