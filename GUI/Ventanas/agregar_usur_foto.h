#ifndef AGREGAR_USUR_FOTO_H
#define AGREGAR_USUR_FOTO_H

#include <QDialog>
#include "Camara.hpp"
#include <QVector>
#include "QTimer"
#include "RostrosDector.hpp"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include "qcamera.h"

namespace Ui {
class agregar_usur_Foto;
}

class agregar_usur_Foto : public QDialog
{
    Q_OBJECT

public:
    explicit agregar_usur_Foto(QWidget *parent = 0);
    ~agregar_usur_Foto();


signals:
    void RaspicamLista(cv::Mat&);
    void CambioValorNumDeFotos();
    void RaspicamRostroDectado(cv::Mat&);
    void RaspicamRostroNODectado();

private slots:
    void on_Btn_TomarFoto_clicked(); //captura la imagen de un rostro
    bool SL_cambiarDeCamara(int IndiceComboBox); //cambia de camara  segun el indice que se escoja
    void SL_Desplegar_camaraLocal(); //despliega lo visualizado por una comara local
    void SL_Desplegar_Raspicam(Mat &); //despliega lo visualizado por la raspicam o camara remota
    void SL_Inhabilitar_Botones(); //garantiza que se tenga justamente 8 fotos
    void on_Btn_Eliminar_clicked(); //elimina la foto selecionada
    void SL_Habilitar_Btn_EliminarFoto(QItemSelection);
    void SL_RostroDetectado(Mat &);
    void SL_RostroNODetectado();

private:
    Ui::agregar_usur_Foto *ui;
    Camara* camaraLocal;
    DectorRostros* DetectorCamLocal;
    QVector<cv::Mat> foto;
//    QVector <QByteArray> IDscamLocal; //id's de las camaras locales
    QTimer* Temporizador;
    QVector<QRgb> TablaDColores;
    int NumDeFotos;
    cv::Mat RostroDetectado;
    QStandardItemModel *modelo;

    QVector<QStandardItem*> capturas;

    QPixmap convertir_Mat_a_Pixmap(Mat &);

};

#endif // AGREGAR_USUR_FOTO_H
