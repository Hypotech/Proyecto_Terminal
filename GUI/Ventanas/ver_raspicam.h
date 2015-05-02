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
    void abrirPuerta();

private slots:
    void DesplegarFrame(const cv::Mat& Frame);
    void on_Btn_AbrirPuerta_clicked();

private:
    Ui::ver_raspicam *ui;
    QVector<QRgb> TablaDColores;
};

#endif // VER_RASPICAM_H
