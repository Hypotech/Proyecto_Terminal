#ifndef AGREGAR_USUR_FINALIZAR_H
#define AGREGAR_USUR_FINALIZAR_H

#include <QDialog>
#include "ojosdetector.h"
#include <QMessageBox>
#include <unistd.h>
#include <QMouseEvent>
#include <QProcess>
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <manejoarchivos.hpp>
#include <QVector>
#include <QStatusBar>
#include <QTextEdit>
#include <iostream>

#define B_N 1

namespace Ui {
class agregar_usur_finalizar;
}

class agregar_usur_finalizar : public QDialog
{
    Q_OBJECT

public:
    explicit agregar_usur_finalizar(QVector<cv::Mat>& Rostros, QWidget *parent = 0);
    ~agregar_usur_finalizar();

signals:
    void cambioValornumFotos();

private slots:
    void on_Btn_Recortar_clicked();
    void on_Btn_Siguiente_clicked();
    void on_Btn_Finalizar_clicked();
    void SL_InhabilitarBotones();

    void on_Btn_Deshacer_clicked();

private:
    Ui::agregar_usur_finalizar *ui;
    cv::Point ojoIzq, ojoDer;
    QVector<QRgb> TablaDColores;
    ojosDetector detectorOjos;
    QVector<cv::Mat>& rostrosEntrada;
    int numFotos;
    std::vector<int> compresion_param;
    cv::Mat imagenRecortada;
    cv::Mat RostroOjosDetec;
    std::string almacenCarpeta;
    QVector<cv::Mat> imagenes_a_Guardar;
    QStatusBar* statusBar;

    bool eventFilter(QObject *obj, QEvent *event);
    QPixmap convertir_Mat_a_Pixmap(cv::Mat& ImagenEntrada);
    bool almacenarImagen();
    void encotrarOjos(cv::Mat& iEntrada, cv::Mat &iSalida);
};

#endif // AGREGAR_USUR_FINALIZAR_H
