#ifndef VENTANAQUITAR_H
#define VENTANAQUITAR_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include "manejoarchivos.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QMessageBox>

namespace Ui {
class ventanaQuitar;
}

class ventanaQuitar : public QDialog
{
    Q_OBJECT

public:
    explicit ventanaQuitar(QWidget *parent = 0);
    ~ventanaQuitar();

private slots:
    void SL_Habilitar_Btn_EliminarFoto();
    void on_Btn_Eliminar_clicked();

private:
    Ui::ventanaQuitar *ui;
    QStandardItemModel *modelo;
    QVector<QStandardItem*> listaFotosUsuarios;
};

#endif // VENTANAQUITAR_H
