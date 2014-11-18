#include "ver_raspicam.h"
#include "ui_ver_raspicam.h"

ver_raspicam::ver_raspicam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ver_raspicam)
{
    ui->setupUi(this);

    for (int i=0; i<256; i++)
        TablaDColores.push_back(qRgb(i,i,i));
}

ver_raspicam::~ver_raspicam()
{
    delete ui;
}

void ver_raspicam::DesplegarFrame(const cv::Mat &Frame)
{
    if(Frame.type()==CV_8UC1)
    {
        QImage imagenQT(
                    (const uchar*)Frame.data,
                    Frame.cols,
                    Frame.rows,
                    Frame.step,
                    QImage::Format_Indexed8);

        imagenQT.setColorTable(TablaDColores);
        ui->lbl_ImagenVideo->setPixmap(QPixmap::fromImage(imagenQT));
    }

    else if(Frame.type()==CV_8UC3)
    {
        QImage imagenQT(
                    (const unsigned char*)(Frame.data),
                    Frame.cols,
                    Frame.rows,
                    Frame.step,
                    QImage::Format_RGB888 );

        imagenQT = imagenQT.rgbSwapped();
        ui->lbl_ImagenVideo->setPixmap(QPixmap::fromImage(imagenQT));
    }
}


void ver_raspicam::on_Btn_AbrirPuerta_clicked()
{
    if ( QMessageBox::warning (this,"Confirme por favor..", "¿Realmente quiere abrir la puerta?",
                         QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes )
        emit abrirPuerta();
}
