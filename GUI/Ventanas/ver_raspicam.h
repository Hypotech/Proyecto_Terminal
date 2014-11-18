/**
 * @author Hipólito Morales Christian
 * @class ver_raspicam
 * @brief Clase para el despliegue de la raspicam
 * @
*/

#ifndef VER_RASPICAM_H
#define VER_RASPICAM_H

#include <QDialog>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <qmessagebox.h>

namespace Ui {
class ver_raspicam;
}

class ver_raspicam : public QDialog
{
    Q_OBJECT

public:
    explicit ver_raspicam(QWidget *parent = 0);
    ~ver_raspicam();

signals:
    /**
     * @brief Esta señal es emitida cuando el usuario acepta abrir a puerta.
     * @details La señal sólo es emitida si el usuario hace click en el botón "Abrir puerta" y
     * responde afirmativamente en el cuadro de dialogo: "¿Realmente quiere abrir la puerta?"
     * @warning La señal no implementa la funcionalidad de apertura de la puerta, sino que, sólo
     * avisa que el usuario acepto abrir la puerta.
     * @see on_Btn_AbrirPuerta_clicked()
     * @see SL_abrirPuerta()
     */
    void abrirPuerta();

private slots:
    /**
     * @brief Slot que despliega imagenes en la ventana "Ver Raspicam".
     * @param [in] Frame Matriz que contiene la imagen a desplegar
     */
    void DesplegarFrame(const cv::Mat& Frame);
    /**
     * @brief Este Slot emite la señal abrirPuerta.
     * @details despliega que un cuadro de dialogo para preguntar al usuario por la apertura
     * de la puerta cuando se hace click en el botón "Abrir puerta", la señal abrirPuerta será
     * emitida si se responde afrimativamente en este cuadro de dialogo.
     * @see abrirPuerta()
     */
    void on_Btn_AbrirPuerta_clicked();

private:
    Ui::ver_raspicam *ui;
    QVector<QRgb> TablaDColores; /**< QVector con los valores QRGB */
};

#endif // VER_RASPICAM_H
