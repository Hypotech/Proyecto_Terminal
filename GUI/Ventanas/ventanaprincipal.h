#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "ver_raspicam.h"
#include "agregar_usuario.h"
#include "QTimer"
#include <Camara.hpp>
#include "RostrosDector.hpp" //funciones para detectar un rostro
#include "ReconocerPersona.hpp" //funciones para reconocer rostro
#include "ReceptorDeImag.hpp" //raspicam remota
#include <ostream>
#include <vector>
#include <QMessageBox>
#include <qstring.h>
#include <ventana_config.h>
#include <ventanaquitar.h>
#include <QFile>
#include <QRegExpValidator>
#include <QRegExp>
#include "agregar_usur_finalizar.h"

namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();

signals:
    void FrameListo();
    void RaspicamLista(const cv::Mat&);
    void RostroDectado(cv::Mat&);
    void RostroNODectado();

private slots:
    void on_Menu_actionSalir_triggered();
    void on_Btn_VerRaspcam_clicked();
    void on_Btn_Agregar_Usr_clicked();
    void SL_ObtenerFrame();
    void SL_ReconocerRostros();
    void on_Btn_Conectar_clicked();
    void on_LEditIP_textEdited(const QString &arg1);
    void on_Btn_Desconectar_clicked();
    void on_Btn_Cofiguracion_clicked();
    void on_Btn_Quitar_Usr_clicked();
    void on_LEditIP_returnPressed();
    void SL_abrirPuerta();

private:
    Ui::VentanaPrincipal *ui;
    QVector<QRgb> TablaDColores;
    QRegExp iPRegEx;
    ReceptorDeImagen Servidor;
    DectorRostros DectorRostrosServidor;
    ReconocerdordePersona* ReconocedorServidor;
    ventana_config::tamanyo minRostro = ventana_config::_200;
    float rangoConfianza = 100.0;
    int Neigen = 8, radio = 1, regX = 8, regY = 8/*, vecinos = 8*/;
    ReconocerdordePersona::metodo modelo = ReconocerdordePersona::LBPH;
    QTimer *Timer;

    cv::Mat FrameRecibido;
    cv::Mat FrameProcesado;
    int RangoConfianza;

    bool LeerArchivoConfig();
};

#endif // VENTANAPRINCIPAL_H
