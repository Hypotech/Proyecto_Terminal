#ifndef VENTANA_CONFIG_H
#define VENTANA_CONFIG_H

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <ReconocerPersona.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace Ui {
class ventana_config;
}

class ventana_config : public QDialog
{
    Q_OBJECT

public:
    enum tamanyo {_200,_150,_100};
    explicit ventana_config(ReconocerdordePersona::metodo metdo = ReconocerdordePersona::LBPH,
                             tamanyo tam = _200, QWidget *parent = 0 );
    ~ventana_config();

private slots:
    void on_Btn_Cancelar_clicked();
    void on_Btn_Aceptar_clicked();

private:
    Ui::ventana_config *ui;
    QString modeloRecon;
    int minRostro;
    bool GuardarConfig();
};

#endif // VENTANA_CONFIG_H
